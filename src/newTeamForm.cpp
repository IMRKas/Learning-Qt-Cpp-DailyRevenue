#include "NewTeamForm.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

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
	teamName = new QLineEdit(this);
	teamCommissioner = new QLineEdit(this);
	teamContactNumber = new QLineEdit(this);

	teamFormLayout->addRow("Nome",teamName);
	teamFormLayout->addRow("Encarregado",teamCommissioner);
	teamFormLayout->addRow("Contato",teamContactNumber);

	// Connects
	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			if(teamName->text().isEmpty()){
				QMessageBox::critical(this,"ERRO","Insira um nome valido para equipe"); // Invalid name error
			} else {
				QSqlQuery insertNewTeam;
				insertNewTeam.prepare("INSERT INTO teams(name, commissioner, contact_number) VALUES (UPPER(?), UPPER(?), ?);");
				insertNewTeam.addBindValue(teamName->text());
				insertNewTeam.addBindValue((teamCommissioner->text().isEmpty()) ? QVariant(QVariant::String) : QVariant(teamCommissioner->text()));
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
	setWindowTitle("ATUALIZAR EQUIPE"); //UPDATE TEAM
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
	teamName = new QLineEdit(this);
	teamCommissioner = new QLineEdit(this);
	teamContactNumber = new QLineEdit(this);

	teamFormLayout->addRow("Nome",teamName);
	teamFormLayout->addRow("Encarregado",teamCommissioner);
	teamFormLayout->addRow("Contato",teamContactNumber);

	// Selected Team data
	QSqlQuery teamInfo;
	teamInfo.prepare("SELECT name, commissioner, contact_number FROM teams WHERE id = ?");
	teamInfo.addBindValue(teamID);

	if(teamInfo.exec() && teamInfo.next()){
		teamName->setText(teamInfo.value(0).toString());
		teamCommissioner->setText(teamInfo.value(1).toString());
		teamContactNumber->setText((teamInfo.value(2).isNull()) ? "" : teamInfo.value(2).toString());
	}

	// Connects
	connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	connect(saveButton, &QPushButton::clicked, this, [=](){
			if(teamName->text().isEmpty()){
				QMessageBox::critical(this,"ERRO","Insira um nome valido para equipe"); // Invalid name error
			} else {
				QSqlQuery insertNewTeam;
				insertNewTeam.prepare("UPDATE teams SET name = UPPER(?), commissioner = UPPER(?), contact_number = ? WHERE id = ?;");
				insertNewTeam.addBindValue(teamName->text());
				insertNewTeam.addBindValue((teamCommissioner->text().isEmpty()) ? QVariant(QVariant::String) : QVariant(teamCommissioner->text()));
				insertNewTeam.addBindValue((teamContactNumber->text().isEmpty()) ? QVariant(QVariant::String) : QVariant(teamContactNumber->text()));
				insertNewTeam.addBindValue(teamID);

				if(!insertNewTeam.exec()){
					QMessageBox::critical(this, "ERRO", "Falha ao atualizar nova equipe:\n" + insertNewTeam.lastError().text()); 
					this->close();
					return;
				} else {
					QMessageBox::information(this,"SUCESSO","Equipe atualizada com sucesso"); 
					this->close();
					return;
					}
			}
			});
}
