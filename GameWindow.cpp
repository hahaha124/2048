#include "GameWindow.h"
#include <QRandomGenerator>
#include <QMessageBox>
GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_score(0)
    , m_BestScore(0)
    , m_gameOver(false)
    , m_gameWon(false)
{
    setWindowTitle("2048");
    initColorMap();
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setFocus();
    mainLayout = new QVBoxLayout(centralWidget);

    titleLabel = new QLabel("2048", centralWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titelFont("Arial", 36, QFont::Bold);
    titleLabel->setFont(titelFont);
    mainLayout->addWidget(titleLabel);

    scoreLayout = new QHBoxLayout();
    scoreLabel = new QLabel("分数: 0", centralWidget);
    QFont scoreFont("Arial", 16, QFont::Bold);
    scoreLabel->setFont(scoreFont);
    scoreLayout->addWidget(scoreLabel);

    bestSoreLabel = new QLabel("最高分: 0", centralWidget);
    bestSoreLabel->setFont(scoreFont);
    scoreLayout->addWidget(bestSoreLabel);

    mainLayout->addLayout(scoreLayout);

    gridLayout = new QGridLayout();
    gridLayout->setSpacing(10);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tileLabels[i][j] = new QLabel("", centralWidget);
            tileLabels[i][j]->setAlignment(Qt::AlignCenter);
            tileLabels[i][j]->setFixedSize(120, 120);
            tileLabels[i][j]->setStyleSheet("background-color: #cdc1b4; border-radius: 5px; font-size: 24px; font-weight: bold;");
            gridLayout->addWidget(tileLabels[i][j], i, j);
        }
    }
    mainLayout->addLayout(gridLayout);

    buttonLayout = new QHBoxLayout();

    newGameButton = new QPushButton("新游戏", centralWidget);
    newGameButton->setFixedSize(150, 50);
    newGameButton->setStyleSheet("QPushButton { background-color: #8f7a66; color: white; font-size: 18px; font-weight: bold; border-radius: 5px; }"
                                 "QPushButton:hover { background-color: #9f8a76; }");
    connect(newGameButton, &QPushButton::clicked, this, &GameWindow::newGame);
    buttonLayout->addWidget(newGameButton);

    mainLayout->addLayout(buttonLayout);

    QLabel *instructions = new QLabel("使用方向键或WASD移动方块，相同数字的方块会合并！", centralWidget);
    instructions->setAlignment(Qt::AlignCenter);
    QFont insFont("Arial", 12);
    instructions->setFont(insFont);
    mainLayout->addWidget(instructions);

    initGame();
}

GameWindow::~GameWindow()
{
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(m_gameOver && !m_gameWon)
    {
        return;
    }
    int key = event->key();
    bool moved = false;

    switch (key) {
    case Qt::Key_Up:
    case Qt::Key_W:
        moved = moveTiles(0, -1);
        mergeTiles(0, -1);
        moved = moveTiles(0, -1) || moved;
        break;

    case Qt::Key_Down:
    case Qt::Key_S:
        moved = moveTiles(0, 1);
        mergeTiles(0, 1);
        moved = moveTiles(0, 1) || moved;
        break;

    case Qt::Key_Left:
    case Qt::Key_A:
        moved = moveTiles(-1, 0);
        mergeTiles(-1, 0);
        moved = moveTiles(-1, 0) || moved;
        break;

    case Qt::Key_Right:
    case Qt::Key_D:
        moved = moveTiles(1, 0);
        mergeTiles(1, 0);
        moved = moveTiles(1, 0) || moved;
        break;

    default:
        QMainWindow::keyPressEvent(event);
        return;
    }
    if (moved) {
        addRandomTile();
        updateUI();
        checkGameState();
    }
}

void GameWindow::newGame()
{
    m_gameOver = false;
    m_BestScore = std::max(m_BestScore,m_score);
    scoreLabel->setText(QString("最高分: %1").arg(m_BestScore));
    initGame();
    setFocus();
}

void GameWindow::updateUI()
{
    scoreLabel->setText(QString("分数: %1").arg(m_score));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int value = m_board[i][j];

            if (value == 0) {
                tileLabels[i][j]->setText("");
                tileLabels[i][j]->setStyleSheet(QString("background-color: %1; border-radius: 5px; font-size: 24px; font-weight: bold;")
                                                .arg(m_colorMap[0]));
            } else {
                tileLabels[i][j]->setText(QString::number(value));

                QString color = m_colorMap.contains(value) ? m_colorMap[value] : "#3c3a32";
                QString textColor = m_textColorMap.contains(value) ? m_textColorMap[value] : "#f9f6f2";

                int fontSize = 24;
                if (value >= 1000) fontSize = 20;
                if (value >= 10000) fontSize = 16;

                tileLabels[i][j]->setStyleSheet(QString("background-color: %1; color: %2; border-radius: 5px; "
                                                        "font-size: %3px; font-weight: bold;")
                                                .arg(color).arg(textColor).arg(fontSize));
            }
        }
    }
}

void GameWindow::checkGameState()
{
    if(m_gameWon)
    {
        QMessageBox::information(this, "恭喜！", "你成功合成2048！\n继续游戏尝试获取更高分数");
    }
    else if(!canMove())
    {
        m_gameOver = true;
        bool hasEmpty = false;
        for(int i = 0; i < 4 && !hasEmpty; i++)
        {
            for(int j = 0; j < 4 && !hasEmpty; j++)
            {
                if(m_board[i][j] == 0)
                {
                    hasEmpty = true;
                }
            }
        }
        if (!hasEmpty) {
            QMessageBox::information(this, "游戏结束", QString("游戏结束！\n你的得分: %1").arg(m_score));
        }
    }
}

void GameWindow::initGame()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            m_board[i][j] = 0;
        }
    }
    addRandomTile();
    addRandomTile();

    updateUI();
}

void GameWindow::initColorMap()
{
    // 初始化方块颜色映射
    m_colorMap[0] = "#cdc1b4";
    m_colorMap[2] = "#eee4da";
    m_colorMap[4] = "#ede0c8";
    m_colorMap[8] = "#f2b179";
    m_colorMap[16] = "#f59563";
    m_colorMap[32] = "#f67c5f";
    m_colorMap[64] = "#f65e3b";
    m_colorMap[128] = "#edcf72";
    m_colorMap[256] = "#edcc61";
    m_colorMap[512] = "#edc850";
    m_colorMap[1024] = "#edc53f";
    m_colorMap[2048] = "#edc22e";

    // 初始化文字颜色映射
    m_textColorMap[0] = "#776e65";
    m_textColorMap[2] = "#776e65";
    m_textColorMap[4] = "#776e65";
    m_textColorMap[8] = "#f9f6f2";
    m_textColorMap[16] = "#f9f6f2";
    m_textColorMap[32] = "#f9f6f2";
    m_textColorMap[64] = "#f9f6f2";
    m_textColorMap[128] = "#f9f6f2";
    m_textColorMap[256] = "#f9f6f2";
    m_textColorMap[512] = "#f9f6f2";
    m_textColorMap[1024] = "#f9f6f2";
    m_textColorMap[2048] = "#f9f6f2";
}

void GameWindow::addRandomTile()
{
    QList<QPair<int, int>> emptyCell;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(m_board[i][j] == 0)
            {
                emptyCell.append(qMakePair(i, j));
            }
        }
    }
    if(emptyCell.empty())
    {
        return;
    }
    int index = QRandomGenerator::global()->bounded(emptyCell.size());
    QPair<int, int> cell = emptyCell[index];
    int value = (QRandomGenerator::global()->bounded(10) < 9) ? 2 : 4;
    m_board[cell.first][cell.second] = value;
}

bool GameWindow::moveTiles(int dx, int dy)
{
    bool move = false;
    int startRow = (dy > 0) ? 3 : 0;
    int endRow = (dy > 0) ? -1 : 4;
    int rowStep = (dy > 0) ? -1 : 1;

    int startCol = (dx > 0) ? 3 : 0;
    int endCol = (dx > 0) ? -1 : 4;
    int colStep = (dx > 0) ? -1 : 1;

    for(int i = startRow; i != endRow; i += rowStep)
    {
        for(int j = startCol; j != endCol; j += colStep)
        {
            if(m_board[i][j] != 0)
            {
                int newRow = i;
                int newCol = j;

                while(newRow + dy >= 0 && newRow + dy < 4 &&
                       newCol + dx >= 0 && newCol + dx < 4 &&
                       m_board[newRow + dy][newCol + dx] == 0)
                {
                    newRow += dy;
                    newCol += dx;
                }

                if (newRow != i || newCol != j) {
                    m_board[newRow][newCol] = m_board[i][j];
                    m_board[i][j] = 0;
                    move = true;
                }
            }
        }
    }
    return move;
}

void GameWindow::mergeTiles(int dx, int dy)
{
    int startRow = (dy > 0) ? 3 : 0;
    int endRow = (dy > 0) ? -1 : 4;
    int rowStep = (dy > 0) ? -1 : 1;

    int startCol = (dx > 0) ? 3 : 0;
    int endCol = (dx > 0) ? -1 : 4;
    int colStep = (dx > 0) ? -1 : 1;
    for(int i = startRow; i != endRow; i += rowStep)
    {
        for(int j = startCol; j != endCol; j += colStep)
        {
            if(m_board[i][j] != 0)
            {
                if(i + dy >= 0 && i + dy < 4 &&
                    j + dx >=0 && j +dx < 4 &&
                    m_board[i + dy][j + dx] == m_board[i][j])
                {
                    m_board[i + dy][j + dx] *= 2;
                    m_score += m_board[i + dy][j + dx];

                    if(m_board[i + dy][j + dx] == 2048 && !m_gameWon)
                    {
                        m_gameWon = true;
                    }

                    m_board[i][j] = 0;
                    int newRow = i + dy;
                    int newCol = j + dx;

                    while(newRow + dy >= 0 && newRow +dy < 4 &&
                           newCol + dx >= 0 && newCol +dx < 4 &&
                           m_board[newRow + dy][newCol + dx] == 0)
                    {
                        m_board[newRow + dy][newCol + dx] = m_board[newRow][newCol];
                        m_board[newRow][newCol] = 0;
                        newRow += dy;
                        newCol += dx;
                    }
                }
            }
        }
    }

}

bool GameWindow::canMove()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j <4; j++)
        {
            if(m_board[i][j] == 0)
            {
                return true;
            }
        }
    }
}
