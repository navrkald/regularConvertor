#ifndef CFGTEST_H
#define CFGTEST_H

#include <QObject>
#include <QtTest/QtTest>
#include <../RegularConvertor/CFG/contextfreegrammar.h>
#include <../RegularConvertor/error.h>

class CfgTest : public QObject
{
  Q_OBJECT
public:
  explicit CfgTest(QObject *parent = 0);
  QString m_backusNaurFrom1;
  CContextFreeGrammar m_grammar1;
  CContextFreeGrammar m_grammar1Check;

signals:

private slots:
  void initTestCase();
  void CfgFromBackusNaurForm();
};

#endif // CFGTEST_H
