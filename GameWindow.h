#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    void keyPressEvent(QKeyEvent* event);

private slots:
    void newGame();
    void updateUI();
    void checkGameState();

private:
    void initGame();
    void initColorMap();
    void addRandomTile();
    bool moveTiles(int dx, int dy);
    void mergeTiles(int dx, int dy);
    bool canMove();

private:
    QWidget* centralWidget;
    QVBoxLayout *mainLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *scoreLayout;
    QHBoxLayout *buttonLayout;

    QLabel* scoreLabel;
    QLabel* bestSoreLabel;
    QLabel* titleLabel;
    QPushButton* newGameButton;

    QLabel* tileLabels[4][4];

    QMap<int, QString> m_colorMap;
    QMap<int, QString> m_textColorMap;

    int m_board[4][4];
    int m_score;
    int m_BestScore;
    bool m_gameOver;
    bool m_gameWon;
};
#endif // GAMEWINDOW_H
