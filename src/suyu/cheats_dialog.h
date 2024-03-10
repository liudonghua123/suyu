// SPDX-FileCopyrightText: Copyright 2018 yuzu Emulator Project & 2024 suyu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <memory>
#include <QDialog>
#include "core/core.h"

namespace Ui {
class CheatsDialog;
}

class CheatsDialog : public QDialog {
    Q_OBJECT

public:
    explicit CheatsDialog(QWidget* parent, u64 title_id, Core::System& system);
    ~CheatsDialog() override;

private:
    std::unique_ptr<Ui::CheatsDialog> ui;
};
