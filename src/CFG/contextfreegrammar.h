#ifndef CONTEXTFREEGRAMMAR_H
#define CONTEXTFREEGRAMMAR_H

#include <QVector>
#include <QPointer>
#include <QSet>
#include <error.h>

class CSymbol
{

public:

  typedef enum{
    symbol,
    terminal,
    nonTerminal
  } TType;

    CSymbol() : m_symbol(""), m_type(symbol) {}
    CSymbol(QString s) : m_symbol(s), m_type(symbol) {}
    CSymbol (const CSymbol& s) { m_symbol = s.m_symbol; m_type = s.m_type; }
    QString GetString() const {return m_symbol;}
    operator QString() const { return m_symbol; }
    static QSet<QString> CSymbolQSetToQStringQSet(const QSet<CSymbol>& csymbolSet);
    bool operator==(const CSymbol& s) const {return this->m_symbol == s.m_symbol ;}
    CSymbol& operator=(CSymbol other) { this->m_symbol = other.m_symbol; return *this; }
    bool IsEmpty() const {return m_symbol.isEmpty();}
    void Trim() {m_symbol = m_symbol.trimmed();}
    void Append(QChar charter) {m_symbol.append(charter);}
    void Clear() {m_symbol.clear();}
    TType GetType() const {return m_type;}
	QDataStream& WriteToQDataStream(QDataStream& out) const;
	QDataStream& ReadFromQDataStream(QDataStream& in);
public:
    QString m_symbol;
    TType m_type;
};

QDataStream& operator>>(QDataStream& in, CSymbol& s);
QDataStream& operator<<(QDataStream& out, const CSymbol& s);

inline uint qHash(const CSymbol& c) {return ::qHash(c.GetString());}

class CTerminal : public CSymbol
{
public:
  CTerminal() : CSymbol() {m_type = terminal;}
  CTerminal(QString symbol) : CSymbol(symbol) {m_type = terminal;}
  static QSet<QString> CTerminalQSetToQStringQSet(const QSet<CTerminal>& csymbolSet);
};

QDataStream& operator>>(QDataStream& in, CTerminal& s);
QDataStream& operator<<(QDataStream& out, const CTerminal& s);

class CNonTerminal : public CSymbol
{
public:
  CNonTerminal() : CSymbol() { m_type = nonTerminal; }
  CNonTerminal(QString symbol) : CSymbol(symbol) { m_type = nonTerminal; }
  static QSet<QString> CNonTerminalQSetToQStringQSet(const QSet<CNonTerminal>& csymbolSet);
};

QDataStream& operator>>(QDataStream& in, CNonTerminal& s);
QDataStream& operator<<(QDataStream& out, const CNonTerminal& s);

class CCFGRule
{
public:
    CCFGRule(){}
    CCFGRule(CNonTerminal leftNonTerminal, QVector<CSymbol> rightString)
        :m_leftNonTerminal(leftNonTerminal), m_rightString(rightString)
    {}

    ~CCFGRule(){}
    static ErrorCode GetRulesFromString(QSet<CCFGRule>& rules, QString sRule);
        bool operator==(const CCFGRule &r) const {
            return ((this->m_leftNonTerminal == r.m_leftNonTerminal) && (this->m_rightString == r.m_rightString));
        }
    void SetLeftNonTerminal(CNonTerminal leftNonTerminal) {m_leftNonTerminal = leftNonTerminal;}
    QString ToString() const;
    QString ToHtml() const;
protected:
    bool IsRightSideEmpty() {return m_rightString.isEmpty();}
    void ClearRightSide() {m_rightString.clear();}
    void AppendSymbolToRightSide(CSymbol symbol) {m_rightString.push_back(symbol);}
public:
    CNonTerminal m_leftNonTerminal;
    QVector<CSymbol> m_rightString;
    QVector<QString> GetRevertedRightRule();
	QDataStream& WriteToQDataStream(QDataStream &out) const;
	QDataStream& ReadFromQDataStream(QDataStream &in);
};

QDataStream& operator<<(QDataStream &out, const CCFGRule& r);
QDataStream& operator>>(QDataStream &in, CCFGRule& r);

inline uint qHash(const CCFGRule& c){
        return (::qHash(c.m_leftNonTerminal.GetString()) /*^ ::qHash(c.m_rightString)*/);
    }

class CContextFreeGrammar
{

public:
    CContextFreeGrammar();
    QSet<CTerminal> GetTerminalAlphabet() const { return m_terminalsAlphabet; }
    QString TerminalAlphabetToString() const;
    QString NonTerminalAlphabetToString() const;
    QSet<CNonTerminal> GetNonTerminalAlphabet() const { return m_nonTerminalsAlphabet; }
    QSet<QString> GetBothTerminalAndNonterminalAlphabet();
    QSet<CCFGRule> GetRules() const { return m_rules; }
    QString GetStartNonTerminal() const { return m_startNonTerminal; }
    int GetRulesCount();
    void SetNonterminalsAlphabet(const QSet<CNonTerminal> &nonterminalsAlphabet) {m_nonTerminalsAlphabet = nonterminalsAlphabet;}
    void SetTerminalsAlphabet(const QSet<CTerminal> &terminalsAlphabet) {m_terminalsAlphabet = terminalsAlphabet;}
    void SetStartNonTerminal(const CNonTerminal &startNonTerminal) {m_startNonTerminal = startNonTerminal; }
    void SetRules(const QSet<CCFGRule> &rules) {m_rules = rules;}
    ErrorCode GetFromBackusNaurForm(QString sContextFreeGrammar);
    QString ToBackusNaurForm() const;
    bool operator==(const CContextFreeGrammar& g)const;
	void Clear();
	QDataStream & WriteToQDataStream(QDataStream & stream) const;
	QDataStream & ReadFromQDataStream(QDataStream & stream);

protected:
    QSet<CNonTerminal> m_nonTerminalsAlphabet;
    QSet<CTerminal> m_terminalsAlphabet;
    CNonTerminal m_startNonTerminal;
    QSet<CCFGRule> m_rules;

    QList<CCFGRule> GetListOfRules() const;
};

QDataStream &operator<<(QDataStream& out, const CContextFreeGrammar& g);
QDataStream &operator >> (QDataStream &in, CContextFreeGrammar& g);


//bool operator==(const CContextFreeGrammar& g1, const CContextFreeGrammar& g2);
#endif // CONTEXTFREEGRAMMAR_H
