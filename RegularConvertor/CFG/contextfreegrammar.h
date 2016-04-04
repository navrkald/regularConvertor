#ifndef CONTEXTFREEGRAMMAR_H
#define CONTEXTFREEGRAMMAR_H

#include <QVector>
#include <QPointer>
#include <QSet>

class CSymbol
{
public:
    QString symbol;
};

class CTerminal : public CSymbol
{};

class CNonTerminal : public CSymbol
{};

typedef QPointer<CSymbol> CSymbolPtr;

class CCFGRule
{
public:
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
};

class CContextFreeGrammar
{

public:
    CContextFreeGrammar();
    QSet<QString> GetTerminalAlphabet();
    QSet<QString> GetNoonTerminalAlphabet();
    QSet<QString> GetBothTerminalAndNonterminalAlphabet();

protected:
    QSet<CNonTerminal> m_nonterminalsAlphabet;
    QSet<CTerminal> m_terminalsAlphabet;
    QSet<CCFGRule> m_rules;
    CNonTerminal m_startNonTerminal;


};

#endif // CONTEXTFREEGRAMMAR_H
