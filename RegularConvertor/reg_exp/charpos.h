#ifndef CHARPOS_H
#define CHARPOS_H

#include <QString>
#include <QList>

//Struktura ve které jsou uloženy data v uzlu stromu reprezentujici regulární výraz

struct CharPos
{
    QString charter;
    int pos;
    bool terminal;
    bool bold;

    //konstruktor pro tuto strukturu
    CharPos(QString _charter  = "",int _pos = -1, bool _terminal = "true", bool _bold = false)
    {
        charter = _charter;
        pos = _pos;
        terminal = _terminal;
        bold = _bold;
    }
};

//operator nad strukturou
bool operator==(const CharPos& c1, const CharPos& c2);

//pomocná konstanta dolaru, ktery s pouziva pri syntakticke analyze RV
const CharPos dolar("$",-1,true, false);

#endif // CHARPOS_H
