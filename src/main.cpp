#include <QApplication>
#include <QSqlDatabase>
#include <QDir>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "WindowMain.h"
#include "DailyRevenueForm.h"

void createTableDailyRevenue();
void createTableTeams();
void createTableFleet();

int main(int argc, char *argv[]){
	QApplication app(argc, argv);

	
	QDir().mkpath("database");
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("database/daily_revenue_db.db");
	if(!db.open()){
		QMessageBox::critical(nullptr,"ERROR","FALHA AO ABRIR BANCO DE DADOS" + db.lastError().text());
		return -1;
	}
	createTableDailyRevenue();
	createTableTeams();
	createTableFleet();

	//WindowMain windowMain;
	//windowMain.show();
	DailyRevenueForm* drForm = new DailyRevenueForm();
	drForm->show();

	return app.exec();
}


void createTableDailyRevenue(){
	QSqlQuery queryCreateTableDailyRevenue;
	queryCreateTableDailyRevenue.prepare("CREATE TABLE IF NOT EXISTS daily_revenue("
										"id INTEGER PRIMARY KEY AUTOINCREMENT,"
										"project_number INTEGER NOT NULL,"
										"date TEXT NOT NULL,"
										"id_team INTEGER NOT NULL,"
										"total_daily_revenue INTEGER NOT NULL,"
										"revenue_diff INTEGER,"
										"goal_achieved TEXT NOT NULL,"
										"responsible_sector TEXT,"
										"goal_unachieved_why TEXT,"
										"FOREIGN KEY (id_team) REFERENCES teams(id));");

	if(!queryCreateTableDailyRevenue.exec()){
		QMessageBox::critical(nullptr, "ERRO", "Falha ao criar tabela daily_production:\n" + queryCreateTableDailyRevenue.lastError().text());
		return;
	}
}

void createTableTeams(){
	QSqlQuery queryCreateTableTeams;
	queryCreateTableTeams.prepare("CREATE TABLE IF NOT EXISTS teams("
							   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
							   "team_name TEXT NOT NULL,"
							   "team_status TEXT NOT NULL,"
							   "team_commissioner TEXT,"
							   "team_daily_revenue_goal INTEGER NOT NULL,"
							   "id_fleet INTEGER UNIQUE,"
							   "team_contact_number INTEGER,"
							   "FOREIGN KEY (id_fleet) REFERENCES fleet(id) ON DELETE SET NULL);");
	if(!queryCreateTableTeams.exec()){
		QMessageBox::critical(nullptr,"ERRO","Falha ao criar tabela teams:\n" + queryCreateTableTeams.lastError().text());
		return;
	}
}

void createTableFleet(){
	QSqlQuery queryCreateFleet;
	queryCreateFleet.prepare("CREATE TABLE IF NOT EXISTS fleet("
							 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
							 "fleet_number INTEGER NOT NULL,"
							 "fleet_type TEXT NOT NULL,"
							 "license_plate TEXT NOT NULL,"
							 "fleet_status TEXT NOT NULL,"
							 "team_using TEXT,"
							 "status_observation TEXT);");
	if(!queryCreateFleet.exec()){
		QMessageBox::critical(nullptr, "ERRO", "Falha ao criar tabela 'fleet':\n" + queryCreateFleet.lastError().text());
		return;
	}
}

