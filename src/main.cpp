#include <QApplication>
#include <QSqlDatabase>
#include <QDir>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "WindowMain.h"
#include "TeamWindow.h"
#include "FleetWindow.h"

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
	createTableDailyProduction();
	createTableTeams();
	createTableFleet();

	FleetWindow* newFleet = new FleetWindow();
	newFleet->show();

	return app.exec();
}


void createTableDailyRevenue(){
	QSqlQuery queryCreateTableDailyRevenue;
	queryCreateTableDailyRevenue.prepare("CREATE TABLE IF NOT EXISTS daily_revenue("
										"id INTEGER PRIMARY KEY AUTOINCREMENT,"
										"date TEXT NOT NULL,"
										"id_team INTEGER NOT NULL,"
										"project_number INTEGER NOT NULL,"
										"total_daily_revenue INTEGER NOT NULL,"
										"goal_achieved TEXT NOT NULL,"
										"goal_unachieved_why TEXT,"
										"goal_unachieved_description TEXT,"
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
							   "name TEXT NOT NULL,"
							   "commissioner TEXT NOT NULL,"
							   "contact_number TEXT,"
							   "id_fleet INTEGER UNIQUE,"
							   "FOREIGN KEY (id_fleet) REFERENCES fleet(id));");
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
							 "status_observation TEXT);");
	if(!queryCreateFleet.exec()){
		QMessageBox::critical(nullptr, "ERRO", "Falha ao criar tabela 'fleet':\n" + queryCreateFleet.lastError().text());
		return;
	}
}
