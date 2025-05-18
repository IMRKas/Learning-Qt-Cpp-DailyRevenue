#include "DailyRevenueWindow.h"
#include "Namespaces.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>

DailyRevenueWindow::DailyRevenueWindow(QWidget* parent) : QWidget(parent){
	setWindowTitle("PRODUÇÃO DIÁRIA");
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts
	dailyTable = new QTableView;
	dailyModel = new QSqlQueryModel; dailyTable ->setModel(dailyModel);
	QVBoxLayout* dailyWindowLayout = new QVBoxLayout(this);
	QHBoxLayout* menuBar = new QHBoxLayout;
	

	// Buttons, ComboBox, QDateEdit
	newDailyRevenue = new QPushButton("Inserir Produção", this);
	updateDailyRevenue = new QPushButton("Atualizar Produção", this);
	deleteDailyRevenue = new QPushButton("Excluir Produção", this);
	filterByTeam = new QComboBox(this);
	filterByReason = new QComboBox(this);
	filterByDate = new QDateEdit(this);

	// Layouts Settings
	menuBar->addWidget(newDailyRevenue);
	menuBar->addWidget(updateDailyRevenue);
	menuBar->addWidget(deleteDailyRevenue);
	menuBar->addWidget(filterByDate);
	menuBar->addWidget(filterByTeam);
	menuBar->addWidget(filterByReason);

	dailyWindowLayout->addLayout(menuBar);
	dailyWindowLayout->addWidget(dailyTable);

	// ComboBox settings
	filterByReason->addItem("");
	filterByReason->addItem(ShortfallReasons::LackOfMaterial, ShortfallReasons::LackOfMaterial);
	filterByReason->addItem(ShortfallReasons::InterjourneyTeam, ShortfallReasons::InterjourneyTeam);
	filterByReason->addItem(ShortfallReasons::EmergencyService, ShortfallReasons::EmergencyService);
	filterByReason->addItem(ShortfallReasons::PerformFailure, ShortfallReasons::PerformFailure);  
	filterByReason->addItem(ShortfallReasons::PlainningFailure, ShortfallReasons::PlainningFailure); 
	filterByReason->addItem(ShortfallReasons::LowRevenueProgramming, ShortfallReasons::LowRevenueProgramming);
	filterByReason->addItem(ShortfallReasons::DifficultiesDueToRain, ShortfallReasons::DifficultiesDueToRain);  
	filterByReason->addItem(ShortfallReasons::LowRevenueJob, ShortfallReasons::LowRevenueJob);
	filterByReason->addItem(ShortfallReasons::DifficultiesDueToComplexity, ShortfallReasons::DifficultiesDueToComplexity);
	filterByReason->addItem(ShortfallReasons::TeamBreak, ShortfallReasons::TeamBreak);

	filterByTeam->addItem("");
	QSqlQuery getTeams;
	getTeams.prepare("SELECT id, team_name FROM teams ORDER BY team_name ASC;");
	if(getTeams.exec()){
		while(getTeams.next()){
			int id = getTeams.value(0).toInt();
			QString teamName = getTeams.value(1).toString();
			filterByTeam->addItem(teamName,id);
		}
	} else {
		QMessageBox::critical(this, "ERRO", "Falha ao acessar equipes:\n" + getTeams.lastError().text());
		return;
	}

	// Connects

	connect(deleteDailyRevenue, &QPushButton::clicked, this, &DailyRevenueWindow::deleteDailyRevenueById);
	connect(updateDailyRevenue, &QPushButton::clicked, this, &DailyRevenueWindow::updateDailyRevenueById);
	connect(newDailyRevenue, &QPushButton::clicked, this, [=](){
			if(!formIsOpen){
				formIsOpen = new DailyRevenueForm();
				formIsOpen->show();

				connect(formIsOpen, &QObject::destroyed, this, [this](){
						this->formIsOpen = nullptr;	
						showDailyRevenue();
						});
				}
			});
 
	showDailyRevenue();
	dailyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void DailyRevenueWindow::deleteDailyRevenueById(){
	int id = getRowId();
	if(!formIsOpen && id >= 0){
		QSqlQuery deleteRow;
		if(QMessageBox::information(this, "ATENÇÃO", "Tem certeza que deseja excluir esta linha ?",
										  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
			deleteRow.prepare("DELETE FROM daily_revenue WHERE id = ?;");
			deleteRow.addBindValue(id);
			if(!deleteRow.exec()){
				QMessageBox::critical(this, "ERRO", "Falha ao excluir linha:\n" + deleteRow.lastError().text());
				return;
			} 
				QMessageBox::information(this, "SUCESSO", "Linha excluida com sucesso!");
				showDailyRevenue();
		}
	}
}

void DailyRevenueWindow::updateDailyRevenueById(){
	int id = getRowId();
	if(!formIsOpen && id >= 0){
		formIsOpen = new DailyRevenueForm(id);
		formIsOpen->show();

		connect(formIsOpen, &QObject::destroyed, this, [this](){
				this->formIsOpen = nullptr;
				showDailyRevenue();
				});
	}
}

void DailyRevenueWindow::showDailyRevenue(){
	dailyModel->setQuery("SELECT daily_revenue.id, daily_revenue.date, daily_revenue.project_number, teams.team_name,"
						 "printf('R$ %.2f', daily_revenue.total_daily_revenue/100.0),"
						 "printf('R$ %.2f', daily_revenue.revenue_diff/100.0), daily_revenue.responsible_sector, "
						 "daily_revenue.goal_unachieved_why "
						 "FROM daily_revenue LEFT JOIN teams ON daily_revenue.id_team = teams.id "
						 "ORDER BY daily_revenue.date DESC;");
	qDebug() << dailyModel->lastError().text();
	dailyModel->setHeaderData(0,Qt::Horizontal,"ID");
	dailyModel->setHeaderData(1,Qt::Horizontal,"DATA");
	dailyModel->setHeaderData(2,Qt::Horizontal,"OV/NOTA");
	dailyModel->setHeaderData(3,Qt::Horizontal,"EQUIPE");
	dailyModel->setHeaderData(4,Qt::Horizontal,"PRODUÇÃO");
	dailyModel->setHeaderData(5,Qt::Horizontal,"DIFERENÇA");
	dailyModel->setHeaderData(6,Qt::Horizontal,"RESPONSÁVEL");
	dailyModel->setHeaderData(7,Qt::Horizontal,"MOTIVO");

	dailyTable->resizeColumnsToContents();
	dailyTable->resizeRowsToContents();
}

int DailyRevenueWindow::getRowId(){
	QModelIndex index = dailyTable->currentIndex();
	if(!index.isValid()) { QMessageBox::warning(this, "ATENÇÃO", "Selecione uma linha para atualizar"); return -1; }

	int id = dailyModel->data(dailyModel->index(index.row(), 0)).toInt();
	return id;
}

