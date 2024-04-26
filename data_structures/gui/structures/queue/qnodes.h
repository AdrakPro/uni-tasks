#ifndef GUI_Q_NODES_H
#define GUI_Q_NODES_H

class QNode {
private:
	int index;
	int value;
	int priority;

public:
	QNode() = default;

	QNode(int index, int value, int priority);

	[[nodiscard]] int getValue() const;

	[[nodiscard]] int getPriority() const;

	[[nodiscard]] int getIndex() const;

	void setPriority(int new_priority);
};

#endif //GUI_Q_NODES_H
