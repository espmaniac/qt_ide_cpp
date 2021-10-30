#include "completelistwidget.h"

CompleteListWidget::CompleteListWidget(QWidget *parent):QListWidget(parent)
{
  p=(QPlainTextEdit*)parent;
  backgroundColor=Qt::lightGray;
  highlightColor.setRgb(22,165,248);
  QPalette palette=this->palette();
  palette.setColor(QPalette::Active,QPalette::Highlight,highlightColor);
  palette.setColor(QPalette::Inactive,QPalette::Highlight,highlightColor);
  palette.setColor(QPalette::Active, QPalette::Base,backgroundColor);
  palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
  palette.setColor(QPalette::Text,Qt::white);
  this->setPalette(palette);
}
void CompleteListWidget::keyPressEvent(QKeyEvent *event){
  if(event->key()==16777235||event->key()==16777237){
      QListWidget::keyPressEvent(event);
    }else{
      QApplication::sendEvent(p,event);
      p->setFocus();
    }
}
int CompleteListWidget::ldistance(const std::string source, const std::string target){
          int n = source.length();
          int m = target.length();
          if (m == 0) return n;
          if (n == 0) return m;
          typedef vector< vector<int> >  Tmatrix;
          Tmatrix matrix(n + 1);
          for (int i = 0; i <= n; i++)  matrix[i].resize(m + 1);

          for (int i = 1; i <= n; i++) matrix[i][0] = i;
          for (int i = 1; i <= m; i++) matrix[0][i] = i;

          for (int i = 1; i <= n; i++)
          {
                  const char si = source[i - 1];
                  for (int j = 1; j <= m; j++)
                  {
                          const char dj = target[j - 1];
                          int cost;
                          if (si == dj){
                                  cost = 0;
                          }
                          else{
                                  cost = 1;
                          }
                          const int above = matrix[i - 1][j] + 1;
                          const int left = matrix[i][j - 1] + 1;
                          const int diag = matrix[i - 1][j - 1] + cost;
                          matrix[i][j] = min(above, min(left, diag));
                  }
          }//step7
          return matrix[n][m];

}
