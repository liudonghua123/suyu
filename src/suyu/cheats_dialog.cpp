// SPDX-FileCopyrightText: Copyright 2018 yuzu Emulator Project & 2024 suyu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <memory>

#include <QHeaderView>
#include <QIcon>
#include <QMenu>
#include <QStandardItemModel>
#include <QString>
#include <QTimer>
#include <QTreeView>
#include <fmt/format.h>
#include <qnamespace.h>
#include <qstringlistmodel.h>

#include "core/core.h"
#include "core/file_sys/patch_manager.h"
#include "suyu/cheats_dialog.h"
#include "suyu/uisettings.h"
#include "ui_cheatsdialog.h"

CheatsDialog::CheatsDialog(QWidget* parent, u64 title_id, Core::System& system)
    : QDialog(parent), ui{std::make_unique<Ui::CheatsDialog>()} {
    // Load the patches before-hand
    FileSys::PatchManager pm(title_id, system.GetFileSystemController(),
                             system.GetContentProvider());
    auto patches = pm.GetPatches();

    // Configure the UI to ensure it is right
    ui->setupUi(this);

    // Configure the cheats list for it to be uniform
    ui->cheats_list->setAlternatingRowColors(true);
    ui->cheats_list->setSelectionMode(QHeaderView::SingleSelection);
    ui->cheats_list->setSelectionBehavior(QHeaderView::SelectRows);
    ui->cheats_list->setVerticalScrollMode(QHeaderView::ScrollPerPixel);
    ui->cheats_list->setHorizontalScrollMode(QHeaderView::ScrollPerPixel);
    ui->cheats_list->setSortingEnabled(true);
    ui->cheats_list->setEditTriggers(QHeaderView::NoEditTriggers);
    ui->cheats_list->setUniformRowHeights(true);
    ui->cheats_list->setContextMenuPolicy(Qt::NoContextMenu);

    // Configure the headers
    ui->cheats_list->header()->setStretchLastSection(false);
    ui->cheats_list->header()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
    ui->cheats_list->header()->setMinimumSectionSize(150);

    // Populate the items
    QList<QTreeWidgetItem*> items;
    for (const FileSys::Patch& patch : patches) {
        const QStringList columns{
            QString(tr("%1")).arg(QString::fromStdString(patch.name)),
            QString(tr("%1")).arg(QString::fromStdString(patch.version)),
        };

        QTreeWidgetItem* patch_item = new QTreeWidgetItem(columns);
        patch_item->setFlags(patch_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        patch_item->setCheckState(0, patch.enabled ? Qt::Checked : Qt::Unchecked);

        items.push_back(patch_item);
    }

    ui->cheats_list->insertTopLevelItems(0, items);
}

CheatsDialog::~CheatsDialog() = default;
