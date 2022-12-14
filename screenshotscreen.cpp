#include "screenshotscreen.h"
#include "ui_screenshotscreen.h"

screenShotScreen::screenShotScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::screenShotScreen)
{
    ui->setupUi(this);
    Taker_a_picture = new screenShot(this);
    creatTabelModel();
}

screenShotScreen::~screenShotScreen()
{
    delete ui;
}

void screenShotScreen::creatTabelModel()
{
    model = new QSqlTableModel(this);
    model->setTable("screenDB");
    model->setHeaderData(0,Qt::Horizontal,"id");
    model->setHeaderData(1,Qt::Horizontal,"Имя изображения");
    model->setHeaderData(2,Qt::Horizontal,"Изображение");
    ui->TableOfPicture->setModel(model);
    ui->TableOfPicture->setColumnHidden(0, true);    // Скрываем колонку с id записей
    ui->TableOfPicture->setColumnHidden(2, true);    // Скрываем колонку с изображением
    // Разрешаем выделение строк
    ui->TableOfPicture->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одной строки в таблице
    ui->TableOfPicture->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->TableOfPicture->resizeColumnsToContents();
    ui->TableOfPicture->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Запрещаем редактирование
    ui->TableOfPicture->horizontalHeader()->setStretchLastSection(true);     // Растягиваем последнюю колонку по всему tableView

    /* Подключаем сигнал об изменении выбора текущей строки в таблицу
     * к СЛОТу для установки изображения в picLabel
     * */
    connect(ui->TableOfPicture->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotCurrentPicture(QModelIndex)));

    model->select(); // Делаем выборку данных из таблицы
}

void screenShotScreen::slotCurrentPicture(QModelIndex index)
{
    QPixmap curPixmap = QPixmap();
    curPixmap.loadFromData(model->data(model->index(index.row(), 2)).toByteArray());
    ui->Picture->setPixmap(curPixmap.scaled(400,300));
}




void screenShotScreen::on_TakeAPick_clicked()
{
   QScreen *screen = QApplication::primaryScreen();    // Берём объект экрана
   QPixmap inPixmap =  screen->grabWindow( 0 );        // Сохраняем его в изображение объекта QPixmap
   QByteArray inByteArray;                             // Создаём объект QByteArray для сохранения изображения
   QBuffer inBuffer( &inByteArray );                   // Сохранение изображения производим через буффер
   inBuffer.open( QIODevice::WriteOnly );              // Открываем буффер
   inPixmap.save( &inBuffer, "PNG" );                  // Записываем inPixmap в inByteArray

   // Записываем скриншот в базу данных
   if (insertIntoTable(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss.png"), inByteArray))
       qDebug() << "Запись произошла успешно";
   else
       qDebug() << "Ошибка записи";

   // Делаем выборку таблицы из Базы Данных
   model->select();

}

bool screenShotScreen::insertIntoTable(const QString &name, const QByteArray &pic)
{
    QVariantList data;
    data.append(name);
    data.append(pic);

    if(insertIntoTable(data))
        return true;
    else
        return false;
}

bool screenShotScreen::insertIntoTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO screenDB ( Name,\
                                             Picture ) \
                  VALUES (:Name, :Pic)");
    query.bindValue(":Name",        data[0].toString());
    query.bindValue(":Pic",         data[1].toByteArray());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into screenDB";
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
