#ifndef CONTEXTFREEGRAMMAR_H
#define CONTEXTFREEGRAMMAR_H

#include <QVector>
#include <QPointer>
#include <QSet>

class CSymbol
{
public:
    CSymbol(){m_symbol="";}
    CSymbol(QString symbol) : m_symbol(symbol) {}
    CSymbol (const CSymbol& s) { m_symbol = s.m_symbol; }
    QString GetString() const {return m_symbol;}
    operator QString() const { return m_symbol; }
    static QSet<QString> CSymbolQSetToQStringQSet(const QSet<CSymbol>& csymbolSet);
    bool operator==(const CSymbol& s) const {return this->m_symbol == s.m_symbol ;}
protected:
    QString m_symbol;
};

inline uint qHash(const CSymbol& c) {return ::qHash(c.GetString());}

class CTerminal : public CSymbol
{
public:
  CTerminal() : CSymbol() {}
  CTerminal(QString symbol) : CSymbol(symbol) {}
  static QSet<QString> CTerminalQSetToQStringQSet(const QSet<CTerminal>& csymbolSet);
};

class CNonTerminal : public CSymbol
{
public:
  CNonTerminal() : CSymbol() {}
  CNonTerminal(QString symbol) : CSymbol(symbol) {}
  static QSet<QString> CNonTerminalQSetToQStringQSet(const QSet<CNonTerminal>& csymbolSet);
};

//typedef CSymbol* CSymbol;

class CCFGRule
{
public:
    CCFGRule(){;}
    CCFGRule(CNonTerminal leftNonTerminal, QVector<CSymbol> rightString)
        :m_leftNonTerminal(leftNonTerminal), m_rightString(rightString)
    {}
//    CCFGRule(QString rule)
//    {
//      QStringList leftAndRightPart = rule.split("->");

//      // Rule has to have 2 parts
//      if(leftAndRightPart.size()!= 2) return;

//      // Get left non-terminal
//      QString leftPart = leftAndRightPart[0];
//      QString leftNonterminal;
//      const QRegExp leftNonTermExp("^s*<s*(\\w+)s*>s*$");
//      if(leftNonTermExp.exactMatch(leftPart))
//      {
//        leftNonTermExp.indexIn(leftPart);
//        leftNonterminal = leftNonTermExp(1);
//      }
//      else
//      { // Left part did not match Backus–Naur Form
//        return;
//      }

//      // Get right rule part
//      //
//      // First split rule with |
//      QStringList rightRules;
//      QVector<QVector<CSymbol>> rightPartRules;
//      foreach(QString rightPartRule , rightRules)
//      {
//        rightPartRule.split("<>")
//      }

//    }

    ~CCFGRule()
    {
        ;
//        foreach(CSymbol symbolPtr, rightString)
//        {
//            delete symbolPtr;
//        }
    }

        bool operator==(const CCFGRule &r) const {
            return ((this->m_leftNonTerminal == r.m_leftNonTerminal) && (this->m_rightString == r.m_rightString));
        }
public:
    CTerminal m_leftNonTerminal;
    QVector<CSymbol> m_rightString;
    QVector<QString> GetRevertedRightRule();
};

inline uint qHash(const CCFGRule& c){
        return (::qHash(c.m_leftNonTerminal.GetString()) /*^ ::qHash(c.m_rightString)*/);
    }

class CContextFreeGrammar
{

public:
    CContextFreeGrammar();
    QSet<CTerminal> GetTerminalAlphabet() const { return m_terminalsAlphabet; }
    QSet<CNonTerminal> GetNoonTerminalAlphabet() const { return m_nonterminalsAlphabet; }
    QSet<QString> GetBothTerminalAndNonterminalAlphabet();
    QSet<CCFGRule> GetRules() const { return m_rules; }
    int GetRulesCount();
    void SetNonterminalsAlphabet(const QSet<CNonTerminal> &nonterminalsAlphabet) {m_nonterminalsAlphabet = nonterminalsAlphabet;}
    void SetTerminalsAlphabet(const QSet<CTerminal> &terminalsAlphabet) {m_terminalsAlphabet = terminalsAlphabet;}
    void SetStartNonTerminal(const CNonTerminal &startNonTerminal) {m_startNonTerminal = startNonTerminal; }
    void SetRules(const QSet<CCFGRule> &rules) {m_rules = rules;}

protected:
    QSet<CNonTerminal> m_nonterminalsAlphabet;
    QSet<CTerminal> m_terminalsAlphabet;
    CNonTerminal m_startNonTerminal;
    QSet<CCFGRule> m_rules;

};

#endif // CONTEXTFREEGRAMMAR_H
