#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include "TeamWindow.h"
#include "FleetWindow.h"
#include "DailyRevenueWindow.h"
#include <QtCharts>


class WindowMain : public QWidget {
	Q_OBJECT
	
	public:
		WindowMain(QWidget* parent = nullptr);
		
		void showGraphDefault(QStringList& setList, QStringList& axisXNames, int range, int tickCount, QString title, QString setTitle);
		void createMainGraph();
		QStringList& getTeamsId();
		QStringList& getTeamsRevenue();
		QStringList& getTotalOfReasons();





	private:
		QPushButton* getFleet{nullptr};
		QPushButton* getTeams{nullptr};
		QPushButton* dailyRevenue{nullptr};
		QPushButton* exportDataToCSV{nullptr};
		QComboBox* filterByTeam{nullptr};
		QComboBox* createGraphBy{nullptr};
		QPushButton* createGraph{nullptr};
		QCheckBox* filterByDate{nullptr};
		QDateEdit* startDate{nullptr};
		QDateEdit* endDate{nullptr};

		TeamWindow* openTeamTable{nullptr};
		FleetWindow* openFleetTable{nullptr};
		DailyRevenueWindow* openDailyTable{nullptr};
		QChartView* graphView{nullptr};
		QChart* graph{nullptr};
		QBarSet* set{nullptr}; 
		QBarSeries* series{nullptr}; 
		QBarCategoryAxis* axisX{nullptr}; 
		QValueAxis* axisY{nullptr}; 

		QStringList teamsID;
		QStringList teamsName;
		QStringList teamsRevenue;
		QStringList totalOfEachReason;
};
#endif
