#include "TeamWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QModelIndex>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>

TeamWindow::TeamWindow(QWidget* parent) : QWidget(parent){
	setWindowTitle("EQUIPES"); // TEAMS
	setAttribute(Qt::WA_DeleteOnClose);

	addNewTeam = new QPushButton("Adicionar nova Equipe", this); 
	deleteTeam = new QPushButton("Excluir Equipe", this);
	updateTeam = new QPushButton("Atualizar Equipe", this);

	QVBoxLayout* teamWindowLayout = new QVBoxLayout(this);
	QHBoxLayout* buttonsRow = new QHBoxLayout;
	teamsView = new QTableView(this);
	model = new QSqlQueryModel(this);
	teamsView->setModel(model);

	buttonsRow->addWidget(addNewTeam);
	buttonsRow->addWidget(updateTeam);
	buttonsRow->addWidget(deleteTeam);
	teamWindowLayout->addLayout(buttonsRow);
	teamWindowLayout->addWidget(teamsView);

	connect(addNewTeam, &QPushButton::clicked, this, [=](){
			if(!newTeam){
				newTeam = new NewTeamForm();
				newTeam->show();

				connect(newTeam, &QObject::destroyed, this, [this](){
						this->newTeam = nullptr;
						showTeams();
				});
			}
	});

	connect(updateTeam, &QPushButton::clicked, this, [=](){
			if(!newTeam){
				QModelIndex teamIndex = teamsView->currentIndex();
				if(!teamIndex.isValid()){
						QMessageBox::information(this, "ERRO", "Selecione uma equipe para atualizar");
						return;
					}
				int teamID = model->data(model->index(teamIndex.row(),0)).toInt();
				newTeam = new NewTeamForm(teamID);
				newTeam->show();

				connect(newTeam, &QObject::destroyed, this, [this](){
						this->newTeam = nullptr;
						showTeams();
						});
				}
			});

	connect(deleteTeam, &QPushButton::clicked, this, &TeamWindow::deleteTeamFromDb);
	showTeams();
	teamsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void TeamWindow::showTeams(){
	model->setQuery("SELECT teams.id, teams.team_name, printf('R$ %.2f', teams.team_daily_revenue_goal/100.0),"
			"fleet.fleet_number, teams.team_status, teams.team_commissioner,"
			"teams.team_contact_number FROM teams LEFT JOIN fleet ON teams.id_fleet = fleet.id ORDER BY teams.team_name ASC;");
	model->setHeaderData(0,Qt::Horizontal,"ID");
	model->setHeaderData(1,Qt::Horizontal,"EQUIPE");
	model->setHeaderData(2,Qt::Horizontal,"META");
	model->setHeaderData(3,Qt::Horizontal,"FROTA");
	model->setHeaderData(4,Qt::Horizontal,"STATUS");
	model->setHeaderData(5,Qt::Horizontal,"ENCARREGADO");
	model->setHeaderData(6,Qt::Horizontal,"CONTATO");

	teamsView->resizeColumnsToContents();
}

void TeamWindow::deleteTeamFromDb(){
	QModelIndex index = teamsView->currentIndex();
	if(!index.isValid()){
		QMessageBox::information(this, "ATENÇÃO", "Selecione uma equipe para excluir");
		return;
	}

	if(QMessageBox::question(this,"EXCLUIR EQUIPE", "TEM CERTEZA QUE DESEJA EXCLUIR ESTA EQUIPE ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
		int teamName = model->data(model->index(index.row(),0)).toInt();

		QSqlQuery queryDeleteTeam;
		queryDeleteTeam.prepare("DELETE FROM teams WHERE id = ?");
		queryDeleteTeam.addBindValue(teamName);

		if(!queryDeleteTeam.exec()){
			QMessageBox::critical(this, "ERRO","Não foi possivel excluir equipe:\n" + queryDeleteTeam.lastError().text());
			return;
		} else {
			QMessageBox::information(this, "SUCESSO","Equipe Excluida com Sucesso");
			showTeams();
			return;
		}
	}
}
