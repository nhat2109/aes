#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int a = 1, b, n;

struct EllipticCurve {
    long long p;  // Modulus
    long long a;  // Hệ số a
    long long b;  // Hệ số b
    long long gx; // Tọa độ x của điểm sinh
    long long gy; // Tọa độ y của điểm sinh
};

int modInverse(int a, int m) {
    int m0 = m;
    int y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        int q = a / m;
        int t = m;
        m = a % m, a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0)
        x += m0;

    return x;
}

bool contains_point(EllipticCurve curve, int x, int y) {
    return (y * y - x * x * x - curve.a * x - curve.b) % curve.p == 0;
}

std::pair<int, int> add_points(EllipticCurve curve, std::pair<int, int> point1, std::pair<int, int> point2) {
    if (point1.first == 0 && point1.second == 0) {
        return point2;
    }
    if (point2.first == 0 && point2.second == 0) {
        return point1;
    }
    if (point1.first == point2.first && point1.second != point2.second) {
        return std::make_pair(0, 0);
    }

    int m;
    if (point1.first == point2.first) {
        m = (3 * point1.first * point1.first + curve.a) * modInverse(2 * point1.second, curve.p) % curve.p;
    } else {
        m = (point1.second - point2.second) * modInverse(point1.first - point2.first, curve.p) % curve.p;
    }

    int x = (m * m - point1.first - point2.first) % curve.p;
    int y = (m * (point1.first - x) - point1.second) % curve.p;
    return std::make_pair(x, y);
}

std::pair<int, int> multiply_point(EllipticCurve curve, std::pair<int, int> point, int n) {
    std::pair<int, int> result = std::make_pair(0, 0);
    std::pair<int, int> addend = point;
    while (n > 0) {
        if (n % 2 == 1) {
            result = add_points(curve, result, addend);
        }
        addend = add_points(curve, addend, addend);
        n /= 2;
    }
    return result;
}

std::pair<int, std::pair<int, int>> generate_key_pair(EllipticCurve curve) {
    // Tạo số ngẫu nhiên để làm khóa riêng
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, curve.p - 1);
    int private_key = dist(gen);

    // Nhân điểm sinh với khóa riêng để tạo khóa công khai
    std::pair<int, int> public_key = multiply_point(curve, std::make_pair(curve.gx, curve.gy), private_key);

    return std::make_pair(private_key, public_key);
}

int EuclidMR(int t, int ri){
    int ri1 = 1;
    while(1){
        if(((ri * ri1) - 1) % t == 0)
            return ri1;
        else
            ri1++;
    }
}

int bpvn(int x, int n, int m){
    int p = 1;
    std::stack<int> st;
    while(n != 0){
        st.push(n % 2);
        n = (n - (n % 2)) / 2;
    }
    while(!st.empty()){
        int i = st.top();
        p = p * p;
        p = p % m;
        if(i == 1){
            p = p * x;
            p = p % m;
        }
        st.pop();
    }
    return p;
}

//------------------------------------
//tab1
bool isPrime(int num) {
    // Kiểm tra các trường hợp đặc biệt
    if (num <= 1) {
        return false;
    }
    if (num == 2) {
        return true;
    }
    // Kiểm tra số lẻ
    if (num % 2 == 0) {
        return false;
    }
    // Kiểm tra các số lẻ khác
    for (int i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int gcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return gcd(b, a % b);
}

int findFiN(int p, int q)
{
    return (p - 1) * (q - 1);
}

int findB(int fiN)
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist1(2, fiN/2), dist2(dist1(rng), fiN - 1);
    int random_number1 = dist1(rng), random_number2 = dist2(rng);
    bool ok = false;
    for (int i = random_number1; i < random_number2; i++)
    {
        if (gcd(i, fiN) == 1)
        {
            ok = true;
            return i;
        }
    }
    if(!ok)
        return findB(fiN);
    return 1;
}

int findA(int fiN)
{
    return EuclidMR(fiN, b);
}


void MainWindow::on_buttonTaoKhoa_clicked()
{
    EllipticCurve curve;
    std::string tempP = "0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff";
    curve.a = -3;
    std::string tempB = "0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b";
    std::string tempGX = "0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    std::string tempGY = "0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";

    curve.p = std::stoi(tempP, nullptr, 16);
    curve.b = std::stoi(tempB, nullptr, 16);
    curve.gx = std::stoi(tempGX, nullptr, 16);
    curve.gy = std::stoi(tempGY, nullptr, 16);

    ui->showP->setText(QString::number(curve.p));

    // Tạo cặp khóa
    //std::pair<int, std::pair<int, int>> key_pair = generate_key_pair(curve);

    //std::cout << "Private key: " << key_pair.first << std::endl;
    //std::cout << "Public key (x, y): (" << key_pair.second.first << ", " << key_pair.second.second << ")" << std::endl;

}

void MainWindow::on_buttonExit_clicked()
{
    QApplication::quit();
}

//------------------------------------
//tab2
void MainWindow::on_buttonExit_2_clicked()
{
    QApplication::quit();
}

void MainWindow::on_taiVB_2_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All files (*.*);; Text File(*.txt);; Word File(*.docx);; Word File(*.doc)")).replace("/", "\\");
    QString text;
    if(!filePath.isEmpty()){
        if(filePath.endsWith(".txt")){
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
              return;
            QTextStream in(&file);
            text = in.readAll();
            file.close();
        } else {
            QAxObject *word = new QAxObject("Word.Application");
            QAxObject *documents = word->querySubObject("Documents");
            QAxObject *document = documents->querySubObject("Open(const QString&)", filePath);
            QAxObject *content = document->querySubObject("Content");

            text = content->dynamicCall("Text()").toString();
            document->dynamicCall("Close()");
            delete content;
            delete document;
            delete documents;
            delete word;
        }
        ui->VBCanKy->setText(text);
    }
}

void MainWindow::on_luuChuKy_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save File", QDir::homePath(), "Text files (*.txt)");
    if (!fileName.isNull())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << ui->chuKy->toPlainText();
            file.close();
        }
    }
}

//------------------------------------
//tab3
void MainWindow::on_taiVB_3_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All files (*.*)")).replace("/", "\\");
    QString text;
    if(!filePath.isEmpty()){
        if(filePath.endsWith(".txt")){
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return;
            QTextStream in(&file);
            text = in.readAll();
            file.close();
        } else {
            QAxObject *word = new QAxObject("Word.Application");
            QAxObject *documents = word->querySubObject("Documents");
            QAxObject *document = documents->querySubObject("Open(const QString&)", filePath);
            QAxObject *content = document->querySubObject("Content");
            text = content->dynamicCall("Text()").toString();
            document->dynamicCall("Close()");
            delete content;
            delete document;
            delete documents;
            delete word;
        }
        ui->VBCanXacNhan->setText(text);
    }
}

void MainWindow::on_xacNhan_clicked()
{
    QString temp_1 = ui->VBCanXacNhan->toPlainText(),
            temp_2 = ui->XacNhanChuKy->toPlainText();
    std::string VBCanXacNhan = temp_1.toStdString(),
                chuKyCanXacNhan = temp_2.toStdString();
    //Kiem tra chu ky
    std::vector<int> numbers;
    std::stringstream ss(chuKyCanXacNhan);
    std::string token;

    while (getline(ss, token, ' ')) {
        int number = std::stoi(token);
        numbers.push_back(number);
    }
    QString hash_1 = "", result = "";
    for (int num : numbers)
    {
        while(num < 0){
            num += n;
        }
        if(b < 0){
            int result_1 = bpvn(num, (-b), n);
            if(result_1 == 0){
                result = "0";
            } else {
                result = QString::number(EuclidMR(n, result_1));
            }
        } else if(b > 0){
            result = QString::number(bpvn(num, b, n));
        } else if (b == 0){
            result = "1";
        }
        std::stringstream stream;
        stream << std::hex << result.toInt();
        std::string hexString = stream.str();
        QString result = QString::fromStdString(hexString);

        hash_1.insert(hash_1.size(), result);
    }
    qDebug() << hash_1;
    if(hash_1 != "hash_2")
        QMessageBox::information(nullptr, "Thông báo", "Chữ ký chính xác");
    else
        QMessageBox::critical(nullptr, "Thông báo", "Chữ ký không chính xác");

}

void MainWindow::on_Chuyen_clicked()
{
    ui->VBCanXacNhan->setText(ui->VBCanKy->toPlainText());
    ui->XacNhanChuKy->setText(ui->chuKy->toPlainText());
}


void MainWindow::on_maHoa_clicked()
{
    QString text = ui->VBCanKy->toPlainText();

    //Tao chu ky
    QString chuKy = "", result = "";
    for (char ch : text.toStdString())
    {
        int hexCode = std::stoi(std::string(1, ch), nullptr, 16);
        if(hexCode == 0){
            result = "0";
        } else if(a < 0){
            int result_1 = bpvn(hexCode, (-a), n);
            if(result_1 == 0){
                result = "0";
            } else {
                result = QString::number(EuclidMR(n, result_1));
            }
        } else if(a > 0){
            result = QString::number(bpvn(hexCode, a, n));
        } else if (a == 0){
            result = "1";
        }
        chuKy.insert(chuKy.size(), result + " ");
    }
    ui->chuKy->setText(chuKy);
}

