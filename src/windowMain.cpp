#include "WindowMain.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

WindowMain::WindowMain(QWidget* parent) : QWidget(parent){
	setWindowTitle("Relatório de Produção Diária");

	// Layout
	QVBoxLayout* mainWindowLayout = new QVBoxLayout(this);
	QHBoxLayout* mainWindowMenuLayout = new QHBoxLayout;

	// MENU - Buttons etc
	getTeams = new QPushButton("EQUIPES", this);
	getFleet = new QPushButton("FROTA", this);
	dailyRevenue = new QPushButton("PRODUÇÃO DIÁRIA", this);


	// Setting Layout;
	mainWindowLayout->addLayout(mainWindowMenuLayout);
	mainWindowMenuLayout->addWidget(dailyRevenue);
	mainWindowMenuLayout->addWidget(getTeams);
	mainWindowMenuLayout->addWidget(getFleet);


	// Setting Connect
	connect(getTeams, &QPushButton::clicked, this, [=](){
			if(!openTeamTable){
				openTeamTable = new TeamWindow();
				openTeamTable->show();

				connect(openTeamTable, &QObject::destroyed, this, [this](){
						this->openTeamTable = nullptr;
						});
			}
	});
	connect(getFleet, &QPushButton::clicked, this, [=](){
			if(!openFleetTable){
				openFleetTable = new FleetWindow();
				openFleetTable->show();

				connect(openFleetTable, &QObject::destroyed, this, [this](){
						this->openFleetTable = nullptr;
						});
				}
			});
}
