#ifndef DAILY_REVENUE_FORM_H
#define DAILY_REVENUE_FORM_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Namespaces.h"

class DailyRevenueForm : public QWidget{
	Q_OBJECT
	public:
		DailyRevenueForm(QWidget* parent = nullptr);
		//DailyRevenueForm(int revenueID, QWidget* parent = nullptr);


	private:
		QFormLayout* dailyForm{nullptr};
		QVBoxLayout* dailyLayout{nullptr};
		QHBoxLayout* buttonsRow{nullptr};
		
		QPushButton* saveButton{nullptr};
		QPushButton* cancelButton{nullptr};

		QDateEdit* revenueDate{nullptr};
		QComboBox* teams{nullptr};
		QLineEdit* projectNumber{nullptr};
		QLineEdit* dailyRevenue{nullptr};
		QLineEdit* goalAchieved{nullptr};
		QLineEdit* diffRevenue{nullptr};
		QComboBox* sectorResponsible{nullptr};
		QComboBox* notAchievedReason{nullptr};

		QLabel* dateLabel{nullptr};
		QLabel* teamLabel{nullptr};
		QLabel* projectLabel{nullptr};
		QLabel* revenueLabel{nullptr};
		QLabel* goalLabel{nullptr};
		QLabel* sectorLabel{nullptr};
		QLabel* reasonLabel{nullptr};
		QLabel* diffLabel{nullptr};

		QHBoxLayout* dateTeamRow{nullptr};
		QHBoxLayout* projectRevenueRow{nullptr};
		QHBoxLayout* reasonRow{nullptr};
};
#endif
