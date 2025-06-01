#include "WindowMain.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QComboBox>
#include <QDateEdit>
#include <QToolTip>
#include <QDebug>

using namespace QtCharts;

WindowMain::WindowMain(QWidget* parent) : QWidget(parent){
	setWindowTitle("Relatório de Produção Diária");
	

	// Layout
	QVBoxLayout* mainWindowLayout = new QVBoxLayout(this);
	QHBoxLayout* mainWindowMenuLayout = new QHBoxLayout;
	QVBoxLayout* graphLayout = new QVBoxLayout;
	QHBoxLayout* graphMenu = new QHBoxLayout;

	// MENU - Buttons etc
	getTeams = new QPushButton("EQUIPES", this);
	getFleet = new QPushButton("FROTA", this);
	dailyRevenue = new QPushButton("PRODUÇÃO DIÁRIA", this);
	createGraphBy = new QComboBox(this);
	createGraph = new QPushButton("Gerar Gráfico", this);
	filterByTeam = new QComboBox(this);
	startDate = new QDateEdit(this);
	endDate = new QDateEdit(this);

	// QDateEdit settings
	QDate currentDate = QDate::currentDate();
	startDate->setDate(QDate(currentDate.year(), currentDate.month(), 1));
	endDate->setDate(currentDate);
	startDate->setCalendarPopup(true);
	endDate->setCalendarPopup(true);
	startDate->setDisplayFormat("dd/MM/yyyy");
	endDate->setDisplayFormat("dd/MM/yyyy");
	startDate->setLocale(QLocale(QLocale::Portuguese, QLocale::Brazil));

	// start Graph
	getTeamsRevenue();
	showGraphDefault(teamsRevenue, teamsName, 2000000, 21, "Produção Diária", "Produção");

	// QComboBox Settings
	createGraphBy->addItem("PRODUÇÃO TOTAL",0);
	createGraphBy->addItem("MEDIA POR DIA",1);
	createGraphBy->addItem("MOTIVOS",2);

	// Setting Layout;
	mainWindowLayout->addLayout(mainWindowMenuLayout);
	mainWindowLayout->addLayout(graphLayout);

	graphLayout->addLayout(graphMenu);
	graphLayout->addWidget(graphView);

	mainWindowMenuLayout->addWidget(dailyRevenue);
	mainWindowMenuLayout->addWidget(getTeams);
	mainWindowMenuLayout->addWidget(getFleet);

	graphMenu->addWidget(startDate);
	graphMenu->addWidget(endDate);
	graphMenu->addWidget(filterByTeam);
	graphMenu->addWidget(createGraphBy);
	graphMenu->addWidget(createGraph);

	// Team ComboBox
	QSqlQuery getTeamsName;
	filterByTeam->addItem("");
	getTeamsName.prepare("SELECT id, team_name FROM teams ORDER BY team_name;");
	if(getTeamsName.exec()){
		while(getTeamsName.next()){
			filterByTeam->addItem(getTeamsName.value(1).toString(),getTeamsName.value(0).toInt());
		}
	}

	// Setting Connect
	connect(createGraph, &QPushButton::clicked, this, &WindowMain::createMainGraph);
	connect(dailyRevenue, &QPushButton::clicked, this, [=](){
			if(!openDailyTable){
				openDailyTable = new DailyRevenueWindow();
				openDailyTable->resize(1366,768);
				openDailyTable->show();

				connect(openDailyTable, &QObject::destroyed, this, [this](){
						this->openDailyTable = nullptr;
				});
			}
	});

	connect(getTeams, &QPushButton::clicked, this, [=](){
			if(!openTeamTable){
				openTeamTable = new TeamWindow();
				openTeamTable->resize(1280,600);
				openTeamTable->show();

				connect(openTeamTable, &QObject::destroyed, this, [this](){
						this->openTeamTable = nullptr;
						});
			}
	});
	connect(getFleet, &QPushButton::clicked, this, [=](){
			if(!openFleetTable){
				openFleetTable = new FleetWindow();
				openFleetTable->resize(1080,600);
				openFleetTable->show();

				connect(openFleetTable, &QObject::destroyed, this, [this](){
						this->openFleetTable = nullptr;
						});
				}
			});

	
}

QStringList& WindowMain::getTeamsId(){
		QSqlQuery getTeamsId;
		if(filterByTeam->currentData().isNull()){
			getTeamsId.prepare("SELECT id,team_name FROM teams WHERE team_status != 'DESMOBILIZADA' ORDER BY team_name;");
			if(getTeamsId.exec()){
				while(getTeamsId.next()){
					this->teamsID << getTeamsId.value(0).toString();
					this->teamsName << getTeamsId.value(1).toString();
				}
			} else {
				QMessageBox::critical(this, "ERRO", "Falha ao acessar dados de equipes :\n" + getTeamsId.lastError().text());
			}
		} else {
			getTeamsId.prepare("SELECT id,team_name FROM teams WHERE id = ?;");
			getTeamsId.addBindValue(filterByTeam->currentData().toInt());
			if(getTeamsId.exec() && getTeamsId.next()){
					this->teamsID << getTeamsId.value(0).toString();
					this->teamsName << getTeamsId.value(1).toString();
			} else {
				QMessageBox::critical(this, "ERRO", "Falha ao acessar dados de equipes :\n" + getTeamsId.lastError().text());
			}
		}

		return teamsID;
}

QStringList& WindowMain::getTeamsRevenue(){
	QSqlQuery getRevenue;
	teamsName.clear();
	teamsID.clear();
	getTeamsId();

	if(filterByTeam->currentData().isNull()){
		getRevenue.prepare("SELECT SUM(total_daily_revenue) FROM daily_revenue WHERE id_team = ? "
						   " AND date >= ? AND date <= ?;");
		for(const auto& id : teamsID){
			getRevenue.addBindValue(id);
			getRevenue.addBindValue(startDate->date().toString(Qt::ISODate));
			getRevenue.addBindValue(endDate->date().toString(Qt::ISODate));
			if(getRevenue.exec() && getRevenue.next()){
				this->teamsRevenue << getRevenue.value(0).toString();
			} else {
				QMessageBox::critical(this, "ERRO", "Falha ao Somar dados do banco de Dados:\n" + 
																getRevenue.lastError().text());
			}
		}

	} else {
		getRevenue.prepare("SELECT SUM(total_daily_revenue) FROM daily_revenue WHERE id_team = ? "
						   " AND date >= ? AND date <= ?;");
		getRevenue.addBindValue(filterByTeam->currentData().toInt());
		getRevenue.addBindValue(startDate->date().toString(Qt::ISODate));
		getRevenue.addBindValue(endDate->date().toString(Qt::ISODate));
		if(getRevenue.exec() && getRevenue.next()){
			this->teamsRevenue << getRevenue.value(0).toString();
		} else {
			QMessageBox::critical(this, "ERRO", "Falha ao Somar dados do banco de Dados:\n" + 
																getRevenue.lastError().text());
		}
	}
		return teamsRevenue;
}

void WindowMain::createMainGraph(){
	QSqlQuery graphQuery;
	teamsRevenue.clear();
	switch(createGraphBy->currentData().toInt()){
		case 0: {
					getTeamsRevenue();
					showGraphDefault(teamsRevenue, teamsName, 2000000, 21, "Produção Total", "Produção");
					break;
				}
		
		case 1: {
					QStringList temp = getTeamsRevenue();
					int days = startDate->date().daysTo(endDate->date());
					teamsRevenue.clear();
					for(const auto& t : temp){
						teamsRevenue << QString::number((t.toLongLong()) / (days));
					}
					temp.clear();
					showGraphDefault(teamsRevenue, teamsName, 20000, 21, "Média Diária","Produção");
					break;
				}

		case 2:{
				   QStringList countReasons;
				   QStringList reasonsName;
				   QSqlQuery reasons;
				   reasons.prepare("SELECT id,reason FROM reasons ORDER BY reason;");
				   if(reasons.exec()){
					   while(reasons.next()){
						   countReasons << reasons.value(0).toString();
						   reasonsName << reasons.value(1).toString();
					   }
				   } else {
						  QMessageBox::critical(this, "ERRO", "Falha ao contar dados de motivos:\n" +
								  								reasons.lastError().text());
				   }
				   QSqlQuery totalReason;
				   if(filterByTeam->currentData().isNull()){
					   for(const auto& id : countReasons){
						  totalReason.prepare("SELECT COUNT(*) FROM daily_revenue WHERE id_reason = ?;");
						  totalReason.addBindValue(id.toInt());

						  if(totalReason.exec() && totalReason.next()){
							  totalOfEachReason << QString::number(totalReason.value(0).toInt() * 100);
						  } else {
							  QMessageBox::critical(this, "ERRO", "Falha ao contar dados de motivos:\n" +
																	totalReason.lastError().text());
						  }
					   }
				   } else {
					   for(const auto& id : countReasons){
						  totalReason.prepare("SELECT COUNT(*) FROM daily_revenue WHERE id_reason = ? AND id_team = ?;");
						  totalReason.addBindValue(id.toInt());
						  totalReason.addBindValue(filterByTeam->currentData().toInt());

						  if(totalReason.exec() && totalReason.next()){
							  totalOfEachReason << QString::number(totalReason.value(0).toInt() * 100);
						  } else {
							  QMessageBox::critical(this, "ERRO", "Falha ao contar dados de motivos:\n" +
																	totalReason.lastError().text());
						  }
					   }
				   }
					showGraphDefault(totalOfEachReason, reasonsName, 200, 21, "Motivos para Produção Baixa", "Nº de Ocorrências");
					countReasons.clear();
					reasonsName.clear();
					totalOfEachReason.clear();
			   }
	}
}

void WindowMain::showGraphDefault(QStringList& setList, QStringList& axisXNames, int range, int tickCount, QString title, QString setText){
	
	set = new QBarSet(setText);

	for(const auto& value : setList){
		 double val = static_cast<double>(value.toLongLong());
			set->append(val / 100.0);
	}

	series = new QBarSeries();
	series->append(set);

	axisX = new QBarCategoryAxis();
	for(const auto& name : axisXNames){
		axisX->append(name);
	}
	axisY = new QValueAxis();
	axisY->setRange(0, range);
	axisY->setTickCount(tickCount);
	axisY->setLabelFormat("%d");

	if(graph){
		graph = graphView->chart();
		graph->removeAllSeries();
		graph->removeAxis(graph->axisX());
		graph->removeAxis(graph->axisY());
	} else {
		graph = new QChart();
		graphView = new QChartView(graph);
	}

	QObject::connect(set, &QBarSet::hovered, [=](bool status, int index){
				if(status){
					if(createGraphBy->currentData() != 2){
						QLocale brasil(QLocale::Portuguese, QLocale::Brazil);
						QToolTip::showText(QCursor::pos(), brasil.toCurrencyString(set->at(index)));
					} else {
						QToolTip::showText(QCursor::pos(), QString::number(set->at(index)));
						}

				}
			});

	graph->addSeries(series);
	graph->setTitle(title);
	graph->legend()->setVisible(true);
	graph->addAxis(axisX, Qt::AlignBottom);
	graph->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);

	graphView->setMouseTracking(true);
	graphView->setRenderHint(QPainter::Antialiasing);
}
