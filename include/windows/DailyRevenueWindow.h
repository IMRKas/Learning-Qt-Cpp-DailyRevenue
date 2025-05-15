#ifndef DAILY_REVENUE_WINDOW_H
#define DAILY_REVENUE_WINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QDateEdit>
#include <QComboBox>

class DailyRevenue : public QWidget{
	Q_OBJECT
	public:
		DailyRevenue(QWidget* parent = nullptr);
		void showDailyRevenue();


	private:
		QPushButton* newDailyRevenue{nullptr};
		QPushButton* updateDailyRevenue{nullptr};
		QPushButton* deleteDailyRevenue{nullptr};
		QComboBox* filterByTeam{nullptr};
		QComboBox* filterByReason{nullptr};

		QDateEdit* filterByDate{nullptr};

		QTableView* dailyTable{nullptr};
		QSqlQueryModel* dailyModel{nullptr};
		

};
#endif

