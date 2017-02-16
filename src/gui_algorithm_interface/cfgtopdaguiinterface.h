#ifndef CCFGTOPDAGUIINTERFACE_H
#define CCFGTOPDAGUIINTERFACE_H

#include <gui_algorithm_interface/cfgtopdaguiinterface.h>
#include <algorithms/algorithm.h>
#include <algorithms/algorithmcfgtopda.h>
#include <QObject>

class CCfgToPdaGuiInterface : public Algorithm
{
  Q_OBJECT
public:
  explicit CCfgToPdaGuiInterface(QObject *parent = 0);
  virtual void InitInstructions();
  virtual void RemoveFuture();

signals:

public slots:
  void prevStep();
  void nextStep();
  void checkSolution();
  void showCorrectSolution();
  void showUserSolution();
  void toBegin();
  void toEnd();

 protected:
  CAlgorithmCFGtoPDA m_algorithm;
};

#endif // CCFGTOPDAGUIINTERFACE_H
