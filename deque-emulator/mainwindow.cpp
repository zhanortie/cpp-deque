#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "algo.h"
#include <algorithm>
#include <QString>
#include <iterator>
#include <random>
#include <cctype>
#include <deque>
#include <string>

static std::deque<std::string> tea{
    "Чай Лунцзин",
    "Эрл Грей",
    "Сенча",
    "Пуэр",
    "Дарджилинг",
    "Ассам",
    "Матча",
    "Ганпаудер",
    "Оолонг",
    "Лапсанг Сушонг"
};

static std::deque<std::string> cakes{
    "Красный бархат",
    "Наполеон",
    "Медовик",
    "Тирамису",
    "Прага",
    "Чизкейк",
    "Захер",
    "Эстерхази",
    "Морковный торт",
    "Чёрный лес"
};

static const auto compare_ci = [](const std::string& l, const std::string& r) {
    return QString::compare(QString::fromStdString(l), QString::fromStdString(r), Qt::CaseInsensitive) < 0;
    };

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    std::random_device rd;
    random_gen_ = std::mt19937(rd());

    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::ApplyModel() {
    auto preserve_iter = deque_model_.iterator;

    ui->list_widget->clear();

    int index = 0;
    for (const auto& item : deque_model_.items) {
        const QString text = QString::number(index) + ": " + QString::fromStdString(item);
        ui->list_widget->addItem(text);
        ++index;
    }

    ui->list_widget->addItem("end");

    deque_model_.iterator = preserve_iter;

    const int size = static_cast<int>(deque_model_.items.size());
    const int pos = static_cast<int>(std::distance(deque_model_.items.begin(), deque_model_.iterator));
    if (pos < 0 || pos > size) {
        deque_model_.iterator = deque_model_.items.begin();
    }

    ui->txt_size->setText(QString::number(size));

    const bool is_empty = deque_model_.items.empty();
    ui->btn_pop_back->setDisabled(is_empty);
    ui->btn_pop_front->setDisabled(is_empty);

    ApplyIterator();
}

void MainWindow::ApplyIterator() {
    auto begin_it = deque_model_.items.begin();
    auto end_it = deque_model_.items.end();

    const bool at_begin = (deque_model_.iterator == begin_it);
    const bool at_end = (deque_model_.iterator == end_it);

    int row = 0;
    if (at_end) {
        row = static_cast<int>(deque_model_.items.size());
    }
    else {
        row = static_cast<int>(std::distance(begin_it, deque_model_.iterator));
    }
    ui->list_widget->setCurrentRow(row);

    ui->btn_edit->setDisabled(at_end);
    ui->btn_erase->setDisabled(at_end);
    ui->btn_iter_inc->setDisabled(at_end);
    ui->btn_iter_dec->setDisabled(at_begin);

    if (at_end) {
        ui->txt_elem_content->setText("");
    }
    else {
        ui->txt_elem_content->setText(QString::fromStdString(*deque_model_.iterator));
    }
}

void MainWindow::on_list_widget_currentRowChanged(int currentRow) {
    const int size = static_cast<int>(deque_model_.items.size());

    if (currentRow < 0) {
        deque_model_.iterator = deque_model_.items.begin();
        ApplyIterator();
        return;
    }

    currentRow = std::clamp(currentRow, 0, size);

    deque_model_.iterator = std::next(deque_model_.items.begin(), currentRow);
    ApplyIterator();
}

void MainWindow::on_btn_push_front_clicked() {
    deque_model_.items.push_front(ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_push_back_clicked() {
    deque_model_.items.push_back(ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_pop_front_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_front();
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_pop_back_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_back();
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_clear_clicked() {
    deque_model_.items.clear();
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_tea_clicked() {
    deque_model_.items = tea;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_cakes_clicked() {
    deque_model_.items = cakes;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_insert_clicked() {
    deque_model_.items.insert(
        deque_model_.iterator,
        ui->txt_elem_content->text().toStdString()
    );

    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_erase_clicked() {
    if (deque_model_.iterator == deque_model_.items.end()) {
        return;
    }

    deque_model_.items.erase(deque_model_.iterator);

    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_edit_clicked() {
    if (deque_model_.iterator == deque_model_.items.end()) {
        return;
    }

    *deque_model_.iterator = ui->txt_elem_content->text().toStdString();
    ApplyModel();
}

void MainWindow::on_btn_begin_clicked() {
    deque_model_.iterator = deque_model_.items.begin();
    ApplyIterator();
}

void MainWindow::on_btn_end_clicked() {
    deque_model_.iterator = deque_model_.items.end();
    ApplyIterator();
}

void MainWindow::on_btn_iter_inc_clicked() {
    if (deque_model_.iterator != deque_model_.items.end()) {
        ++deque_model_.iterator;
    }
    ApplyIterator();
}

void MainWindow::on_btn_iter_dec_clicked() {
    if (deque_model_.iterator != deque_model_.items.begin()) {
        --deque_model_.iterator;
    }
    ApplyIterator();
}

void MainWindow::SetRandomGen(const std::mt19937& random_gen) {
    random_gen_ = random_gen;
}

void MainWindow::on_btn_resize_clicked() {
    bool ok = false;
    int new_size = ui->txt_size->text().toInt(&ok);
    if (!ok) {
        return;
    }

    if (new_size < 0) {
        new_size = 0;
    }
    if (new_size > 1000) {
        new_size = 1000;
    }

    deque_model_.items.resize(static_cast<size_t>(new_size));
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_find_clicked() {
    const std::string value = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::find(deque_model_.items.begin(),
        deque_model_.items.end(),
        value);
    ApplyIterator();
}

void MainWindow::on_btn_count_clicked() {
    const std::string value = ui->le_count->text().toStdString();
    const auto c = std::count(deque_model_.items.begin(),
        deque_model_.items.end(),
        value);
    ui->lbl_count->setText(QString::number(static_cast<int>(c)));
}

void MainWindow::on_btn_min_element_clicked() {
    deque_model_.iterator = std::min_element(deque_model_.items.begin(),
        deque_model_.items.end());
    ApplyIterator();
}

void MainWindow::on_btn_max_element_clicked() {
    deque_model_.iterator = std::max_element(deque_model_.items.begin(),
        deque_model_.items.end());
    ApplyIterator();
}

void MainWindow::on_btn_merge_sort_clicked() {
    deque_model_.items = MergeSort(deque_model_.items, std::less<std::string>());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_merge_sOrT_clicked() {
    deque_model_.items = MergeSort(deque_model_.items, compare_ci);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_shuffle_clicked() {
    std::shuffle(deque_model_.items.begin(), deque_model_.items.end(), random_gen_);
    ApplyModel();
}

void MainWindow::on_btn_reverse_clicked() {
    std::reverse(deque_model_.items.begin(), deque_model_.items.end());
    ApplyModel();
}

void MainWindow::on_btn_unique_clicked() {
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        ApplyModel();
        return;
    }

    auto new_end = std::unique(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.items.erase(new_end, deque_model_.items.end());

    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_lower_bound_clicked() {
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }

    const std::string value = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::lower_bound(deque_model_.items.begin(),
        deque_model_.items.end(),
        value);
    ApplyIterator();
}

void MainWindow::on_btn_upper_bound_clicked() {
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }

    const std::string value = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::upper_bound(deque_model_.items.begin(),
        deque_model_.items.end(),
        value);
    ApplyIterator();
}
