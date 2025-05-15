#ifndef DAILY_REVENUE_WINDOW_H
#define DAILY_REVENUE_WINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QComboBox>

class DailyRevenue : public QWidget{
	Q_OBJECT
	public:
		DailyRevenue(QWidget* parent = nullptr);


	private:
		QPushButton* newDailyRevenue{nullptr};
		QPushButton* updateDailyRevenue{nullptr};
		QPushButton* deleteDailyRevenue{nullptr};
		QComboBox* filterByTeam{nullptr};

		QTableView* showDailyRevenue{nullptr};
		QSqlQueryModel* dailyRevenueModel{nullptr};

};
#endif

