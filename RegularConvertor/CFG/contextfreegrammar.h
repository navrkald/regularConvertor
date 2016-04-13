#ifndef CONTEXTFREEGRAMMAR_H
#define CONTEXTFREEGRAMMAR_H

#include <QVector>
#include <QPointer>
#include <QSet>

class CSymbol
{
public:
    CSymbol(){m_symbol="";}
    QString m_symbol;
};

class CTerminal : public CSymbol
{};

class CNonTerminal : public CSymbol
{};

typedef CSymbol* CSymbolPtr;

class CCFGRule
{
public:
    CCFGRule(){;}
    CCFGRule(CTerminal leftTerminal, QVector<CSymbolPtr> rightString)
        :leftTerminal(leftTerminal), rightString(rightString)
    {}
    ~CCFGRule()
    {
        ;
//        foreach(CSymbolPtr symbolPtr, rightString)
//        {
//            delete symbolPtr;
//        }
    }

public:
    CTerminal leftTerminal;
    QVector<CSymbolPtr> rightString;
    QVector<QString> GetRevertedRightRule();
};

class CContextFreeGrammar
{

public:
    CContextFreeGrammar();
    QSet<QString> GetTerminalAlphabet();
    QSet<QString> GetNoonTerminalAlphabet();
    QSet<QString> GetBothTerminalAndNonterminalAlphabet();
    int GetRulesCount();
    QSet<CCFGRule> m_rules;
protected:
    QSet<CNonTerminal> m_nonterminalsAlphabet;
    QSet<CTerminal> m_terminalsAlphabet;
    CNonTerminal m_startNonTerminal;


};

#endif // CONTEXTFREEGRAMMAR_H
