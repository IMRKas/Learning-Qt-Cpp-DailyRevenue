#include "NewTeamForm.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

NewTeamForm::NewTeamForm(QWidget* parent) : QWidget(parent){
	setWindowTitle("NOVA EQUIPE"); // New Team
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts
	QVBoxLayout* teamWindowLayout = new QVBoxLayout(this);
	QHBoxLayout* buttonsRow = new QHBoxLayout;
	QFormLayout* teamFormLayout = new QFormLayout;

	teamWindowLayout->addLayout(teamFormLayout);
	teamWindowLayout->addLayout(buttonsRow);

	// Buttons
	saveButton = new QPushButton("Salvar", this); 
	cancelButton = new QPushButton("Cancelar", this);

	buttonsRow->addWidget(saveButton);
	buttonsRow->addWidget(cancelButton);

	// Form
	teamName = new QLineEdit(this); teamName->setMaxLength(8);
	teamCommissioner = new QLineEdit(this); teamCommissioner->setMaxLength(25);
	teamContactNumber = new QLineEdit(this);
	teamFleetNumber = new QComboBox(this);
	teamStatus = new QComboBox(this);

	teamFormLayout->addRow("Equipe",teamName);
	teamFormLayout->addRow("Status",teamStatus);
	teamFormLayout->addRow("Encarregado",teamCommissioner);
	teamFormLayout->addRow("Frota", teamFleetNumber);
	teamFormLayout->addRow("Contato",teamContactNumber);

	// ComboBox
	teamFleetNumber->addItem("");
	QSqlQuery getFleetOptions;
	getFleetOptions.prepare("SELECT id, fleet_number FROM fleet WHERE id NOT IN(SELECT id_fleet FROM teams) "
			"AND fleet_status != ? "
			"AND fleet_status != ?;");
	getFleetOptions.addBindValue(DbStatus::broken);
	getFleetOptions.addBindValue(DbStatus::inWorkshop);
	if(getFleetOptions.exec()){
		while(getFleetOptions.next()){
			int id = getFleetOptions.value(0).toInt();
			QString fleetNumber = getFleetOptions.value(1).toString();
			teamFleetNumber->addItem(fleetNumber,id);
		}
	} else {
		QMessageBox::critical(this, "ERRO", "Falha ao recuperar Frotas da tabela fleet:\n" + getFleetOptions.lastError().text());
	}

	teamStatus->addItem(TeamStatus::Working,TeamStatus::Working.toUpper()); 
	teamStatus->addItem(TeamStatus::InactiveTeam,TeamStatus::InactiveTeam.toUpper()); 
	teamStatus->addItem(TeamStatus::DefunctTeam,TeamStatus::DefunctTeam.toUpper()); 

	// Connects
	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			if(teamName->text().isEmpty()){
				QMessageBox::critical(this,"ERRO","Insira um nome valido para equipe"); // Invalid name error
			} if(teamCommissioner->text().isEmpty() && teamStatus->currentText() == TeamStatus::Working){
				QMessageBox::warning(this,"ATENÇÃO", "Para equipes que estão trabalhando, insira o nome do Encarregado");
			} else {
				QSqlQuery insertNewTeam;
				insertNewTeam.prepare("INSERT INTO teams(team_name, team_status, team_commissioner, id_fleet, team_contact_number) VALUES (UPPER(?),?,UPPER(?), ?, ?);");
				insertNewTeam.addBindValue(teamName->text());
				
				insertNewTeam.addBindValue((teamStatus->currentData().isNull()) ? QVariant(QVariant::String) : QVariant(teamStatus->currentData()));

				insertNewTeam.addBindValue(((teamCommissioner->text().isEmpty())||(teamStatus->currentText() == TeamStatus::InactiveTeam || teamStatus->currentText() == TeamStatus::DefunctTeam)) ? QVariant(QVariant::String) : QVariant(teamCommissioner->text()));

				insertNewTeam.addBindValue(((teamStatus->currentText() == TeamStatus::InactiveTeam || teamStatus->currentText() == TeamStatus::DefunctTeam) || teamFleetNumber->currentData().isNull()) ? QVariant(QVariant::String) : QVariant(teamFleetNumber->currentData()));

				insertNewTeam.addBindValue((teamContactNumber->text().isEmpty()) ? QVariant(QVariant::String) : QVariant(teamContactNumber->text()));

				if(!insertNewTeam.exec()){
					QMessageBox::critical(this, "ERRO", "Falha ao inserir nova equipe:\n" + insertNewTeam.lastError().text()); // 'Insert new team' error
					this->close();
					return;
				} else {
					QMessageBox::information(this,"SUCESSO","Nova equipe adicionada com sucesso"); // new team added to db
					this->close();
					return;
					}
			}
			});
}

NewTeamForm::NewTeamForm(int teamID, QWidget* parent) : QWidget(parent){
	setWindowTitle("ATUALIZAR EQUIPE"); // New Team
	setAttribute(Qt::WA_DeleteOnClose);

	// Layouts
	QVBoxLayout* teamWindowLayout = new QVBoxLayout(this);
	QHBoxLayout* buttonsRow = new QHBoxLayout;
	QFormLayout* teamFormLayout = new QFormLayout;

	teamWindowLayout->addLayout(teamFormLayout);
	teamWindowLayout->addLayout(buttonsRow);

	// Buttons
	saveButton = new QPushButton("Salvar", this); 
	cancelButton = new QPushButton("Cancelar", this);

	buttonsRow->addWidget(saveButton);
	buttonsRow->addWidget(cancelButton);

	// Form
	teamName = new QLineEdit(this); teamName->setMaxLength(8);
	teamCommissioner = new QLineEdit(this); teamCommissioner->setMaxLength(25);
	teamContactNumber = new QLineEdit(this);
	teamFleetNumber = new QComboBox(this);
	teamStatus = new QComboBox(this);

	teamFormLayout->addRow("Equipe",teamName);
	teamFormLayout->addRow("Status",teamStatus);
	teamFormLayout->addRow("Encarregado",teamCommissioner);
	teamFormLayout->addRow("Frota", teamFleetNumber);
	teamFormLayout->addRow("Contato",teamContactNumber);

	// ComboBox
	teamFleetNumber->addItem("");
	QSqlQuery getFleetOptions;
	getFleetOptions.prepare("SELECT id, fleet_number FROM fleet WHERE id NOT IN(SELECT id_fleet FROM teams) "
			"AND fleet_status != ? "
			"AND fleet_status != ?;");
	getFleetOptions.addBindValue(DbStatus::broken);
	getFleetOptions.addBindValue(DbStatus::inWorkshop);
	if(getFleetOptions.exec()){
		while (getFleetOptions.next()){
			int id = getFleetOptions.value("id").toInt();
			QString fleetNumber = getFleetOptions.value("fleet_number").toString();
			teamFleetNumber->addItem(fleetNumber,id);
		}
	}
	teamStatus->addItem(TeamStatus::Working,TeamStatus::Working.toUpper()); 
	teamStatus->addItem(TeamStatus::InactiveTeam,TeamStatus::InactiveTeam.toUpper()); 
	teamStatus->addItem(TeamStatus::DefunctTeam,TeamStatus::DefunctTeam.toUpper()); 

	// Get data From fleet and team tables
	QSqlQuery getTeamData;
	getTeamData.prepare("SELECT team_name, team_status, team_commissioner, id_fleet, team_contact_number FROM teams WHERE id = ?;");
	getTeamData.addBindValue(teamID);
	if(getTeamData.exec() && getTeamData.next()){
		teamName->setText(getTeamData.value(0).toString());
		teamStatus->setCurrentText(getTeamData.value(1).toString());
		teamCommissioner->setText(getTeamData.value(2).toString());
		teamContactNumber->setText((getTeamData.value(4).isNull()) ? "" : getTeamData.value(4).toString());

		if(!getTeamData.value(3).isNull()){
			QSqlQuery getFleetNumber;
			getFleetNumber.prepare("SELECT fleet_number FROM fleet WHERE id = ?;");
			getFleetNumber.addBindValue(getTeamData.value(3).toInt());
			if(getFleetNumber.exec() && getFleetNumber.next()){
				teamFleetNumber->setCurrentText(getFleetNumber.value(0).toString());
			}
		} else {
			teamFleetNumber->setCurrentText("");
		}
	}




	// Connects
	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			if(teamName->text().isEmpty()){
				QMessageBox::critical(this,"ERRO","Insira um nome valido para equipe"); // Invalid name error
			} if(teamCommissioner->text().isEmpty() && teamStatus->currentText() == TeamStatus::Working){
				QMessageBox::warning(this,"ATENÇÃO", "Para equipes que estão trabalhando, insira o nome do Encarregado");
			} else {
				QSqlQuery updateSelectedTeam;
				updateSelectedTeam.prepare("UPDATE teams SET team_name = ?, team_status = ?, team_commissioner = ?, id_fleet = ?, team_contact_number = ? WHERE id = ?;");
				updateSelectedTeam.addBindValue(teamName->text());
				updateSelectedTeam.addBindValue(teamStatus->currentData());
				updateSelectedTeam.addBindValue(((teamCommissioner->text().isEmpty())||(teamStatus->currentText() == TeamStatus::InactiveTeam || teamStatus->currentText() == TeamStatus::DefunctTeam)) ? QVariant(QVariant::String) : QVariant(teamCommissioner->text()));
				updateSelectedTeam.addBindValue((teamFleetNumber->currentData().isNull()) ? QVariant(QVariant::Int) : QVariant(teamFleetNumber->currentData()));
				updateSelectedTeam.addBindValue(((teamStatus->currentText() == TeamStatus::InactiveTeam || teamStatus->currentText() == TeamStatus::DefunctTeam) || teamFleetNumber->currentData().isNull()) ? QVariant(QVariant::String) : QVariant(teamContactNumber->text()));
				updateSelectedTeam.addBindValue(teamID);

				if(!updateSelectedTeam.exec()){
					QMessageBox::critical(this, "ERRO", "Falha ao inserir nova equipe:\n" + updateSelectedTeam.lastError().text()); // 'Insert new team' error
					this->close();
					return;
				} else {
					QMessageBox::information(this,"SUCESSO","Informações da Equipe Atualizada."); // new team added to db
					this->close();
					return;
					}
			}
		});
}
