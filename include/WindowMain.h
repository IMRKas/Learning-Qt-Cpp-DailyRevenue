#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>


class WindowMain : public QWidget {
	Q_OBJECT
	
	public:
		WindowMain(QWidget* parent = nullptr);








	private:
		QPushButton* getFleet{nullptr};
		QPushButton* getTeams{nullptr};
		QPushButton* addProductivity{nullptr};
		QPushButton* exportDataToCSV{nullptr};
		QComboBox* filterByEquipe{nullptr};
};
#endif
