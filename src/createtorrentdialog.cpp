/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "createtorrentdialog.h"

#include <QVBoxLayout>

#include "downloadtorrent.h"

CreateTorrentDialog::CreateTorrentDialog()
{
	createButton = new QPushButton(tr("Create Torrent"));
	QVBoxLayout* vert = new QVBoxLayout();
	vert->addWidget(createButton);
	setLayout(vert);
	

	DownloadTorrent x("http://forum.uruchie.org/attachment.php?attachmentid=58701");
	x.download();
}

CreateTorrentDialog::~CreateTorrentDialog()
{

}

#include "createtorrentdialog.moc"