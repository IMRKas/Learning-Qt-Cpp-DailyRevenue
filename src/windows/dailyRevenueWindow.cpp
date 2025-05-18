#include "DailyRevenueWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

DailyRevenueWindow::DailyRevenueWindow(QWidget* parent) : QWidget(parent){
	setWindowTitle("PRODUÇÃO DIÁRIA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts
	dailyTable = new QTableView;
	dailyModel = new QSqlQueryModel; dailyTable ->setModel(dailyModel);
	QVBoxLayout* dailyWindowLayout = new QVBoxLayout(this);
	QHBoxLayout* menuBar = new QHBoxLayout;

	// Buttons, ComboBox, QDateEdit
	newDailyRevenue = new QPushButton(this);
	updateDailyRevenue = new QPushButton(this);
	deleteDailyRevenue = new QPushButton(this);
	filterByTeam = new QComboBox(this);
	filterByReason = new QComboBox(this);
	filterByDate = new QDateEdit(this);
	


}

void DailyRevenueWindow::showDailyRevenue(){
	dailyModel->setQuery("SELECT...");
	dailyModel->setHeaderData(0,Qt::Horizontal,"Data");

}
