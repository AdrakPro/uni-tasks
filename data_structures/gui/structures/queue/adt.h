#ifndef GUI_Q_ADT_H
#define GUI_Q_ADT_H

#include "../nodes.h"

class ADT {
// Zapytac prowadzaca co zrobic, gdy mam dwie funckje, rózniace paramterem
	virtual void insert(const QNode &node) = 0;

};

#endif //GUI_Q_ADT_H
