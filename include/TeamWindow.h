#ifndef WINDOW_EQUIPE_H
#define WINDOW_EQUIPE_H

#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include "NewTeamForm.h"

class TeamWindow : public QWidget {
	Q_OBJECT
	public:
		TeamWindow(QWidget* parent = nullptr);

		void showTeams();
		void deleteTeamFromDb();



	private:
		QPushButton* addNewTeam{nullptr};
		QPushButton* deleteTeam{nullptr};
		QPushButton* updateTeam{nullptr};
		QTableView* teamsView{nullptr};
		QSqlQueryModel* model{nullptr};
		NewTeamForm* newTeam{nullptr};
};

#endif
