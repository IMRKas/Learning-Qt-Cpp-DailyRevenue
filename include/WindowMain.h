#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include "TeamWindow.h"
#include "FleetWindow.h"


class WindowMain : public QWidget {
	Q_OBJECT
	
	public:
		WindowMain(QWidget* parent = nullptr);








	private:
		QPushButton* getFleet{nullptr};
		QPushButton* getTeams{nullptr};
		QPushButton* dailyRevenue{nullptr};
		QPushButton* exportDataToCSV{nullptr};
		QComboBox* filterByEquipe{nullptr};

		TeamWindow* openTeamTable{nullptr};
		FleetWindow* openFleetTable{nullptr};
};
#endif
