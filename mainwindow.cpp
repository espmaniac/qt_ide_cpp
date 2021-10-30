#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setAcceptDrops(true);

    setWindowTitle("CIDEPP");
    SetupTabWidget();
    SetupFileExplorerDock();
    SetupOpenedDocsDock();
    CreateFile();
    SetupMenuBar();

    QCompleter *completer = new QCompleter(QStringList("test"), this);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setWrapAround(false);


    setCentralWidget(tabs);
    tabs->currentWidget()->setFocus();

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent* drag_event) {
    if (drag_event->mimeData()->hasUrls())
        drag_event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* drop_event) {
    QList<QUrl> url_list = drop_event->mimeData()->urls();
    foreach (QUrl url, url_list) {
        OpenFile(url.url(QUrl::RemoveScheme));
    }
}

void MainWindow::SetupTabWidget() {
    tabs = new QTabWidget(this);

    tabs->setStyleSheet("color:#000000; font-size: 12px; font:sans-serif !important;");
    tabs->setMovable(true);
    tabs->setTabsClosable(true);
    tabs->setUsesScrollButtons(true);
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseFile(int)));

    QToolButton* new_tab_button = new QToolButton;
    new_tab_button->setText("+");
    connect(new_tab_button, SIGNAL(clicked()), this, SLOT(CreateFile()));
    tabs->setCornerWidget(new_tab_button, Qt::TopRightCorner);
}

void MainWindow::SetupMenuBar() {
    QMenu* fileMenu = new QMenu("File");

    //QMenu* viewMenu = new QMenu("View");

    fileMenu->addAction("New File",   this, SLOT(CreateFile()),    Qt::CTRL + Qt::Key_N);
    fileMenu->addAction("Open File",  this, SLOT(OpenFile()),      Qt::CTRL + Qt::Key_O);
    fileMenu->addSeparator();
    fileMenu->addAction("Save File",  this, SLOT(SaveFile()),      Qt::CTRL + Qt::Key_S);

    //viewMenu->addAction(file_explorer_dock->toggleViewAction());
    file_explorer_dock->show();
    //viewMenu->addAction(opened_docs_dock->toggleViewAction());
    opened_docs_dock->show();
    ui->menubar->addMenu(fileMenu);
    fileMenu->hide();
    //ui->menubar->addMenu(viewMenu);
}


void MainWindow::SetupFileExplorerDock() {
    file_system_model = new QDirModel;
    file_system_view  = new QTreeView;
    file_system_view->setStyleSheet("font-size: 12px !important; font:sans-serif;");
    file_system_view->setModel(file_system_model);
    file_system_view->setRootIndex(file_system_model->index("/home"));
    file_system_view->setColumnHidden(1, true);
    file_system_view->setColumnHidden(2, true);
    file_system_view->setColumnHidden(3, true);
    file_system_view->setHeaderHidden(true);
    connect(file_system_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OpenFile(QModelIndex)));

    file_explorer_dock = new QDockWidget("File explorer", this);

    file_explorer_dock->setStyleSheet("font-size: 12px !important; font:sans-serif !important;");

    file_explorer_dock->setWidget(file_system_view);

    file_explorer_dock->hide();

    addDockWidget(Qt::RightDockWidgetArea, file_explorer_dock);
}

void MainWindow::SetupOpenedDocsDock() {
    opened_docs_widget = new QListWidget;
    opened_docs_widget->setStyleSheet("font-size: 12px !important; font:sans-serif;");
    connect(tabs->tabBar(),      SIGNAL(tabMoved(int, int)),            this, SLOT(ChangeTabIndexInList(int, int)));
    connect(opened_docs_widget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(UpdateCurrentIndex(QListWidgetItem*)));
    connect(opened_docs_widget, SIGNAL(currentRowChanged(int)),        tabs, SLOT(setCurrentIndex(int)));
    connect(tabs->tabBar(),      SIGNAL(currentChanged(int)),           this, SLOT(UpdateCurrentIndex(int)));
    connect(tabs->tabBar(),      SIGNAL(tabCloseRequested(int)),        this, SLOT(UpdateCurrentIndexOnDelete(int)));

    opened_docs_dock  = new QDockWidget("Opened files", this);
    opened_docs_dock->setStyleSheet("font-size: 12px !important; font:sans-serif !important;");
    opened_docs_dock->setWidget(opened_docs_widget);

    opened_docs_dock->hide();
    addDockWidget(Qt::RightDockWidgetArea, opened_docs_dock);
}

void MainWindow::CreateFile() {
    CodeEditor *new_text_edit = new CodeEditor;
    QCompleter *completer = new QCompleter(new_text_edit->wordlist, this);
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);

    new_text_edit->setCompleter(completer);

    int index = tabs->addTab(new_text_edit, "untitled.cpp");

    tabs->setCurrentIndex(index);
    tabs->setTabToolTip(index, "");
    tabs->setTabWhatsThis(index, "No changes");
    connect(new_text_edit, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));

    QListWidgetItem* new_item = new QListWidgetItem;
    new_item->setText(tabs->tabText(index));

    new_item->setToolTip(tabs->tabToolTip(index));
    opened_docs_widget->addItem(new_item);

    UpdateCurrentIndex(index);
}

void MainWindow::OpenFile() {
    QString filepath = QFileDialog::getOpenFileName(this, "Choose file", "/home");
    if (filepath.isEmpty())
        return;
    OpenFile(filepath);
}

void MainWindow::OpenFile(const QString& filepath) {
    for (int i = 0; i < tabs->count(); ++i)
        if (tabs->tabToolTip(i) == filepath) {
            QMessageBox::warning(this, "Duplicate", "Cannot open one file twice!",
                                          QMessageBox::Ok);
            return;
        }

    QString filename = filepath.section(-1,-1);
    QFile file(filepath);

    if (file.open(QIODevice::ReadOnly)) {
        CodeEditor* temp_text = (CodeEditor*)tabs->currentWidget();
        if (temp_text->document()->isEmpty() &&
            tabs->tabToolTip(tabs->currentIndex()) == "" &&
            tabs->tabText(tabs->currentIndex()) == "untitled") {
            DeleteTabFromList(tabs->currentIndex());
            delete tabs->widget(tabs->currentIndex());
        }

        CodeEditor* new_text_edit = new CodeEditor;

        QString content = file.readAll();
        file.close();
        new_text_edit->appendPlainText(content);
        int index = tabs->addTab(new_text_edit, filename);
        tabs->setCurrentIndex(index);
        tabs->setTabToolTip(index, filepath);
        tabs->setTabWhatsThis(index, "No changes");
        connect(new_text_edit, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));

        QListWidgetItem* new_item = new QListWidgetItem;
        new_item->setText(tabs->tabText(index));
        new_item->setToolTip(tabs->tabToolTip(index));
        opened_docs_widget->addItem(new_item);

        QString file_extension = QFileInfo(filename).suffix();

        tabs->setTabWhatsThis(index, "No changes");
        UpdateCurrentIndex(index);
    } else {
        (new QErrorMessage(this))->showMessage("Cannot open file!");
        return;
    }
}

void MainWindow::SaveFile() {
    if (tabs->tabToolTip(tabs->currentIndex()) == "") {
        SaveFileAs();
        return;
    }
    QString filepath = tabs->tabToolTip(tabs->currentIndex());
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(((CodeEditor*)tabs->currentWidget())->document()->toPlainText().toUtf8());
        file.close();
        tabs->setTabWhatsThis(tabs->currentIndex(), "No changes");
    } else {
        (new QErrorMessage(this))->showMessage("Cannot save file!");
        return;
    }
}

void MainWindow::SaveFileAs() {
    QString filename = tabs->tabText(tabs->currentIndex());
    QString filepath = QFileDialog::getSaveFileName(this, "Save " + filename, "/home/" + filename);
    if (filepath.isEmpty())
        return;
    if (QFileInfo(filepath).suffix().isEmpty())
        filepath.append(".txt");
    else {
        QFile file(filepath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(((CodeEditor*)tabs->currentWidget())->document()->toPlainText().toUtf8());
            file.close();
        } else {
            (new QErrorMessage(this))->showMessage("Cannot save file!");
            return;
        }
    }
    filename = filepath.section("/",-1,-1);
    tabs->tabBar()->setTabText(tabs->currentIndex(), filename);
    tabs->tabBar()->setTabToolTip(tabs->currentIndex(), filepath);

    QString file_extension = QFileInfo(filename).suffix();

    tabs->setTabWhatsThis(tabs->currentIndex(), "No changes");
}

void MainWindow::SaveAllFiles() {
    int current_index = tabs->currentIndex();
    for (int i = 0; i < tabs->count(); ++i) {
        tabs->setCurrentIndex(i);
        if (tabs->tabWhatsThis(tabs->currentIndex()) != "No changes")
            SaveFile();
    }
    tabs->setCurrentIndex(current_index);
}

void MainWindow::CloseFile(int index_) {
    if (tabs->tabWhatsThis(tabs->currentIndex()) != "No changes") {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Saving changes", "Save changes before closing?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            SaveFile();
        }
    }
    delete tabs->widget(index_);

    DeleteTabFromList(index_);

    if (!tabs->count())
        CreateFile();
    tabs->currentWidget()->setFocus();
}

void MainWindow::CloseFile() {
    CloseFile(tabs->currentIndex());
}

void MainWindow::CloseAllFiles() {
    bool checker = false;
    for (int32_t i = 0; i < tabs->count(); ++i) {
        if (tabs->tabWhatsThis(i) != "No changes") {
            checker = true;
            break;
        }
    }
    if (checker) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Save all changes", "Save all changes before closing?",
            QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
            SaveAllFiles();
    }
    while (tabs->count() > 0)
        delete tabs->widget(0);

    opened_docs_widget->clear();

    CreateFile();
    tabs->currentWidget()->setFocus();
}

void MainWindow::CloseWindow() {
    CloseAllFiles();
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent*) {
    CloseWindow();
}

void::MainWindow::UpdateParameter() {
    QString file = tabs->tabBar()->tabText(tabs->currentIndex());
    QString file_extension = QFileInfo(file).suffix();

    tabs->setTabWhatsThis(tabs->currentIndex(), "Changed");
    disconnect(sender(), SIGNAL(textChanged()), this, SLOT(UpdateParameter()));
}


void MainWindow::OpenFile(QModelIndex file_index) {
    if (!file_system_model->isDir(file_index))
        OpenFile(file_system_model->filePath(file_index));
}

void MainWindow::DeleteTabFromList(int index) {
    QListWidgetItem* temp_item = opened_docs_widget->takeItem(index);
    delete temp_item;
}

void MainWindow::ChangeTabIndexInList(int old_index, int new_index) {
    QListWidgetItem* first_item  = opened_docs_widget->takeItem(old_index);
    opened_docs_widget->insertItem(new_index, first_item);
}

void MainWindow::UpdateCurrentIndex(QListWidgetItem* current_item) {
    int index = current_item->listWidget()->row(current_item);
    tabs->setCurrentIndex(index);
}

void MainWindow::UpdateCurrentIndex(int new_selection_index) {
    opened_docs_widget->setCurrentRow(new_selection_index);
    QString file = tabs->tabBar()->tabText(new_selection_index);
    QString file_extension = QFileInfo(file).suffix();
}

void MainWindow::UpdateCurrentIndexOnDelete() {
    opened_docs_widget->setCurrentRow(opened_docs_widget->count() - 1);
}


