#ifndef CHARPOS_H
#define CHARPOS_H

#include <QString>
#include <QList>
#include "finite_machine/computationalrules.h"


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
//    CharPos operator=(const CharPos c)
//    {
//        CharPos r;
//        r.bold = c.bold;
//        r.charter = c.charter;
//        r.pos = c.pos;
//        r.terminal = c.terminal;
//        return r;
//    }
};

//operators above structure
bool operator==(const CharPos& c1, const CharPos& c2);


//pomocná konstanta dolaru, ktery s pouziva pri syntakticke analyze RV
const CharPos dolar("$",-1,true, false);
const CharPos emptyString (EMPTYSET,0,true);
QDataStream &operator<<(QDataStream& out, const CharPos& ch);
QDataStream &operator>>(QDataStream& in, CharPos& ch);

#endif // CHARPOS_H
