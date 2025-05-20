#ifndef DAILY_REVENUE_WINDOW_H
#define DAILY_REVENUE_WINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QDateEdit>
#include <QComboBox>
#include <QCheckBox>
#include "DailyRevenueForm.h"

class DailyRevenueWindow : public QWidget{
	Q_OBJECT
	public:
		DailyRevenueWindow(QWidget* parent = nullptr);
		void showDailyRevenue();
		void updateDailyRevenueById();
		void deleteDailyRevenueById();
		void headerDefinition();
		void filterByAny();
		int getRowId();


	private:
		QPushButton* newDailyRevenue{nullptr};
		QPushButton* updateDailyRevenue{nullptr};
		QPushButton* deleteDailyRevenue{nullptr};
		QComboBox* filterByTeam{nullptr};
		QComboBox* filterByReason{nullptr};
		QCheckBox* willFilterByDate{nullptr};

		QDateEdit* filterByDate{nullptr};

		QTableView* dailyTable{nullptr};
		QSqlQueryModel* dailyModel{nullptr};
		
		DailyRevenueForm* formIsOpen{nullptr};

};
#endif

