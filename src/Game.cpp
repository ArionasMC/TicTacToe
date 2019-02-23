#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "AnimatedObject.h"
#include "Board.h"
#include "MyButton.h"
#include <stdlib.h>
#include <cstdint>
#include <SDL_mixer.h>
#include "AnimatedText.h"
Game::Game()
{}

Game::~Game()
{}

AnimatedObject* animation_o;
AnimatedObject* animation_x;
AnimatedObject* animation_paper_left;
AnimatedObject* animation_paper_right;
SDL_Rect paper_back;

int playerScore = 0;
int computerScore = 0;

MyText* playerText;
MyText* playerScoreText;
MyText* computerText;
MyText* computerScoreText;

MyButton* restartButton;

Mix_Chunk* title_effect = NULL;
Mix_Chunk* map_effect = NULL;

int line_length = 400;

//SDL_Rect line1;
//SDL_Rect line2;
//SDL_Rect line3;
//SDL_Rect line4;

//Point pBoard[3][3];

Board* screenBoard;

struct Move {
	int row, col;
};

char charPlayer = 'x', charOpponent = 'o', charEmpty = '_';

char mainBoard[3][3];

vector<AnimatedObject*> animations;
vector<SDL_Rect> o_rects;

bool playerTurn = false;

TTF_Font* font;
bool addScore = true;

bool menu = true;
bool difficulty = false;
bool inGame = false;

GameObject* tic_title;
GameObject* tac_title;
GameObject* toe_title;

MyButton* playButton;
SDL_Rect playButtonCoat;
int playButtonOpacity;

MyButton* easy;
MyButton* medium;
MyButton* hard;

MyText* easyText;
MyText* mediumText;
MyText* hardText;

MyText* comingSoon1;
MyText* comingSoon2;

int maxDelay = 60;
int delay = 0;

MyText* versionText;
AnimatedText* vt_animation;
MyText* creditsText;
AnimatedText* ct_animation;

// Start of AI functions

bool isMovesLeft(char board[3][3]) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(board[i][j]==charEmpty) return true;
		}
	}
	return false;
}

int evaluate(char board[3][3]) {

	// horizontal check
	for(int row = 0; row < 3; row++) {
		if(board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
			if(board[row][0] == charPlayer)
				return +10;
			else if(board[row][0] == charOpponent)
				return -10;
		}
	}

	// vertical check
	for(int col = 0; col < 3; col++) {
		if(board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
			if(board[0][col] == charPlayer)
				return +10;
			else if(board[0][col] == charOpponent)
				return -10;
		}
	}

	// diagnonal check
	if(board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
		if(board[0][0] == charPlayer)
			return +10;
		else if(board[0][0] == charOpponent)
			return -10;
	}

	if(board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
		if(board[0][2] == charPlayer)
			return +10;
		else if(board[0][2] == charOpponent)
			return -10;
	}

	return 0;
}

int minimax(char board[3][3], int depth, bool isMaximal) {
	int score = evaluate(board);
	//cout << " " << score << " ";

	if(score == 10) return score - depth;
	if(score == -10) return score + depth;

	if(isMovesLeft(board) == 0) return 0;

	//cout << "Running this...\n";

	if(isMaximal) {

		int bestVal = -1000;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {

				if(board[i][j] == charEmpty) {

					board[i][j] = charPlayer;

					int value = minimax(board, depth+1, false);
					bestVal = max(bestVal, value);

					board[i][j] = charEmpty;

				}

			}
		}
		return bestVal;

	} else {

		int bestVal = 1000;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {

				if(board[i][j] == charEmpty) {

					board[i][j] = charOpponent;

					int value = minimax(board, depth+1, true);
					bestVal = min(bestVal, value);

					board[i][j] = charEmpty;
				}

			}
		}
		return bestVal;

	}
}

Move findBestMove(char board[3][3]) {

	int bestVal = -1000;
	Move bestMove;
	bestMove.row = -1;
	bestMove.col = -1;

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j <3; j++) {

			if(board[i][j] == charEmpty) {

				board[i][j] = charPlayer;
				int currentVal = minimax(board, 0, false);
				board[i][j] = charEmpty;

				if(currentVal > bestVal) {
					bestVal = currentVal;
					bestMove.row = i;
					bestMove.col = j;
				}

			}

		}
	}

	return bestMove;

}

bool isThereWinner(char board[3][3]) {
    if(evaluate(mainBoard) == 0) {
        //cout << "draw!";
        return 0;
    } else if(evaluate(mainBoard) == +10) {
        //cout << "Computer wins!";
        return 1;
    } else if(evaluate(mainBoard) == -10) {
        //cout << "You win!";
        return 1;
    }
}

// End of AI functions

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 237, 195, 106, 255);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		}

		isRunning = true;
	}

	if ( TTF_Init( ) == -1 )
    {
        std::cout << "Initialization of TTF failed : " << SDL_GetError() << std::endl;
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    font = TTF_OpenFont("OpenSans-Regular.ttf", 25);

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            mainBoard[i][j] = charEmpty;
        }
    }

    animation_o = new AnimatedObject("assets/spritesheets/animation_o.png", renderer, 50, 50, o_rects, 9, 5);
    animation_o->setDimensions(75, 75);
    animation_o->generateFrameRects(10, 50, 0, 49, 50);

    vector<SDL_Rect> x_rects;
    animation_x = new AnimatedObject("assets/spritesheets/animation_x.png", renderer, 150, 50, x_rects, 9, 5);
    animation_x->setDimensions(75, 75);
    animation_x->generateFrameRects(10, 50, 0, 50, 50);

    vector<SDL_Rect> paper_rects;
    animation_paper_left = new AnimatedObject("assets/spritesheets/animation_paper.png", renderer, 640, 0, paper_rects, 0, 7);
    animation_paper_left->setDimensions(80, 600);
    animation_paper_left->setSpeedX(-5);
    animation_paper_left->generateFrameRects(4, 80, 0, 80, 600);
    animation_paper_right = new AnimatedObject("assets/spritesheets/animation_paper.png", renderer, 720, 0, paper_rects, 0, 7);
    animation_paper_right->setDimensions(80,600);
    animation_paper_right->generateFrameRects(4, 80, 0, 80, 600);
    paper_back.x = 700;//50;
    paper_back.y = 50;//50;
    paper_back.w = 50;//700;
    paper_back.h = 500;//500;

    int cornerX = 200;
    int cornerY = 100;

    screenBoard = new Board(renderer, cornerX, cornerY, line_length);

    /*
    line1.x = cornerX+line_length/3-10;
    line1.y = cornerY;
    line1.w = 10;
    line1.h = 0;
    line2.x = cornerX+line_length-line_length/3;
    line2.y = cornerY;
    line2.w = 10;
    line2.h = 0;
    line3.x = cornerX;
    line3.y = cornerY+line_length/3;
    line3.w = 0;
    line3.h = 10;
    line4.x = cornerX;
    line4.y = cornerY+line_length-line_length/3;
    line4.w = 0;
    line4.h = 10;

    pBoard[0][0].x = cornerX+(line_length/6)-(75/2); pBoard[0][0].y = cornerY+(line_length/6)-(75/2);
    pBoard[0][1].x = pBoard[0][0].x+line_length/3; pBoard[0][1].y = cornerY+(line_length/6)-(75/2);
    pBoard[0][2].x = pBoard[0][1].x+line_length/3; pBoard[0][2].y = cornerY+(line_length/6)-(75/2);

    pBoard[1][0].x = pBoard[0][0].x; pBoard[1][0].y = pBoard[0][0].y+line_length/3;
    pBoard[1][1].x = pBoard[0][1].x; pBoard[1][1].y = pBoard[1][0].y;
    pBoard[1][2].x = pBoard[0][2].x; pBoard[1][2].y = pBoard[1][0].y;

    pBoard[2][0].x = pBoard[0][0].x; pBoard[2][0].y = pBoard[1][0].y+line_length/3;
    pBoard[2][1].x = pBoard[0][1].x; pBoard[2][1].y = pBoard[2][0].y;
    pBoard[2][2].x = pBoard[0][2].x; pBoard[2][2].y = pBoard[2][0].y;
    */

    /*
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            int r = rand() % 100;
            string sheet;
            if(r < 50) sheet = "assets/spritesheets/animation_o.png";
            else sheet = "assets/spritesheets/animation_x.png";
            AnimatedObject* ao = new AnimatedObject(sheet.c_str(), renderer, pBoard[j][i].x, pBoard[j][i].y, o_rects, 9, 5);
            ao->setDimensions(75, 75);
            ao->generateFrameRects(10, 50, 0, 49, 50);
            animations.push_back(ao);
        }
    }
    */

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            cout << "[" << i <<"]["<<j<<"] = {"<<screenBoard->get_rBoard()[i][j].x<<", "<<screenBoard->get_rBoard()[i][j].y<<", "<<screenBoard->get_rBoard()[i][j].w<<", "<<screenBoard->get_rBoard()[i][j].h<<"}\n";
        }
    }

    restartButton = new MyButton("assets/buttons/restart_button1.png", renderer, screenBoard->get_pBoard()[0][0].x + line_length/2 - 180, 500, 300, 71);
    restartButton->setShow(false);

    string p_score = ""+to_string(playerScore);
    playerText = new MyText("You: ", font, 80, 50, {0,0,0}, renderer);
    playerScoreText = new MyText(p_score.c_str(), font, 95, 80, {0,0,0}, renderer);
    string c_score = ""+to_string(computerScore);
    computerText = new MyText("Computer: ", font, 600, 50, {0,0,0}, renderer);
    computerScoreText = new MyText(c_score.c_str(), font, 655, 80, {0,0,0}, renderer);

    tic_title = new GameObject("assets/tic_title.png", renderer, 100, -200);
    tic_title->setSimpleTexture(true);
    tic_title->setShow(true);
    tic_title->setDimensions(148,162);
    tac_title = new GameObject("assets/tac_title.png", renderer, 150, -200);
    tac_title->setSimpleTexture(true);
    tac_title->setShow(true);
    tac_title->setDimensions(173,164);
    toe_title = new GameObject("assets/toe_title.png", renderer, 100, -200);
    toe_title->setSimpleTexture(true);
    toe_title->setShow(true);
    toe_title->setDimensions(173,150);

    playButton = new MyButton("assets/buttons/play_button1.png", renderer, 450, 180, 215, 71);
    playButtonCoat = {450,180,215,71};
    playButtonOpacity = 255;

    easy = new MyButton("assets/buttons/easy1.png", renderer, 100, 150, 160,160);
    easy->setPlayTriggerSound(false);
    medium = new MyButton("assets/buttons/medium1.png", renderer, 320, 150, 160,160);
    medium->setPlayTriggerSound(false);
    hard = new MyButton("assets/buttons/hard1.png", renderer, 540, 150, 160,160);
    hard->setPlayTriggerSound(false);

    easyText = new MyText("EASY", font, 150, 100, {0,255,0}, renderer);
    mediumText = new MyText("MEDIUM", font, 350, 100, {234, 195, 37}, renderer);
    hardText = new MyText("HARD", font, 590, 100, {255,0,0}, renderer);
    comingSoon1 = new MyText("(Coming Soon!)", font, 95, 320, {255,0,0},renderer);
    comingSoon2 = new MyText("(Coming Soon!)", font, 315, 320, {255,0,0},renderer);

    title_effect = Mix_LoadWAV( "assets/sounds/Jump 1.wav" );
    map_effect = Mix_LoadWAV("assets/sounds/map_cut2.wav");

    // Added spaces before text to fix color issue
    versionText = new MyText("  Version: Beta 0.3", font, 510, 450, {0,0,0}, renderer);
    vt_animation = new AnimatedText(versionText, renderer, 5, 45);
    creditsText = new MyText("  Nikolas Spyropoulos © 2018", font, 380, 490, {0,0,0}, renderer);
    ct_animation = new AnimatedText(creditsText, renderer, 5, 0);

}

void Game::handleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
    case SDL_MOUSEBUTTONDOWN:
        //animation_o->restartAnimation();
        //animation_x->restartAnimation();
        //animation_paper->restartAnimation();
        /*for(int i = 0; i < animations.size(); i++) {
            animations[i]->restartAnimation();
        }*/

        if(inGame) {
            int x, y;
            SDL_GetMouseState( &x, &y );
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    if(pointInRect(screenBoard->get_rBoard()[i][j], x, y) && mainBoard[i][j] == charEmpty && playerTurn && !isThereWinner(mainBoard)) {
                        string sheet;
                        if(playerTurn) {
                            sheet = "assets/spritesheets/animation_o.png";
                            mainBoard[i][j] = charOpponent;
                        } else {
                            sheet = "assets/spritesheets/animation_x.png";
                            mainBoard[i][j] = charPlayer;
                        }
                        AnimatedObject* ao = new AnimatedObject(sheet.c_str(), renderer, screenBoard->get_pBoard()[i][j].x, screenBoard->get_pBoard()[i][j].y, o_rects, 9, 5);
                        ao->setDimensions(75, 75);
                        ao->generateFrameRects(10, 50, 0, 49, 50);
                        ao->setSoundEffectSource("assets/sounds/pencil_write.wav");
                        ao->setPlaySoundEffect(true);
                        animations.push_back(ao);
                        playerTurn = !playerTurn;
                        delay = maxDelay;

                    }
                }
            }

            if(restartButton->isShowing() && restartButton->isTriggered(x,y)) {
                //restart code
                animations.clear();
                for(int i = 0; i < 3; i++) {
                    for(int j = 0; j < 3; j++) {
                        mainBoard[i][j] = charEmpty;
                    }
                }
                restartButton->setShow(false);
                addScore = true;
                delay = 0;
            }
        } else if(menu) {
            int x, y;
            SDL_GetMouseState( &x, &y );
            if(playButton->isTriggered(x,y) && playButtonOpacity == 0) {
                menu = false;
                difficulty = true;
            }
        } else if(difficulty) {
            int x, y;
            SDL_GetMouseState( &x, &y );
            if(hard->isTriggered(x,y)) {
                difficulty = false;
                inGame = true;
                animations.clear();
                for(int i = 0; i < 3; i++) {
                    for(int j = 0; j < 3; j++) {
                        mainBoard[i][j] = charEmpty;
                    }
                }
                restartButton->setShow(false);
                addScore = true;
                delay = 0;
            }
        }

        break;
    case SDL_KEYDOWN:

        switch(event.key.keysym.sym) {
            case SDLK_ESCAPE:

                if(difficulty) {
                    difficulty = false;
                    menu = true;
                } else if(inGame) {
                    inGame = false;
                    difficulty = true;
                }

                break;
            default:
                break;
        }

        break;
	default:
		break;
	}
}

bool sound1played = false;
bool sound2played = false;
bool sound3played = false;
bool mapStarted = false;

void Game::update()
{

	//animation_o->update();
	//animation_o->playAnimation();
	//animation_x->update();
	//animation_x->playAnimation();

	if(inGame) {

        for(int i = 0; i < animations.size(); i++) {
            animations[i]->update();
            animations[i]->playAnimation();
        }

        if(paper_back.x > 50) paper_back.x -= 5;
        if(paper_back.w < 700) paper_back.w += 5;

        /*
        if(animation_paper_left->getX()<=0) {
            if(line1.h < line_length) line1.h+=5;
            if(line2.h < line_length) line2.h+=5;
            if(line3.w < line_length) line3.w+=5;
            if(line4.w < line_length) line4.w+=5;
        }
        */
        if(animation_paper_left->getX()<=0) {
            //cout << "Starting animation!\n";
            screenBoard->startAnimation();
        }

        screenBoard->update();
        animation_paper_left->update();
        animation_paper_left->playAnimation();
        animation_paper_right->update();
        animation_paper_right->playAnimation();
        if(animation_paper_left->getAnimated() && animation_paper_left->getX() > 0) {
            animation_paper_left->setAnimated(false);
            animation_paper_right->setAnimated(false);
        }
        if(animation_paper_left->getX() <= 0) animation_paper_left->setSpeedX(0);

        if(animation_paper_left->getAnimated() && !playerTurn && !isThereWinner(mainBoard) && isMovesLeft(mainBoard)) {

            delay--;
            if(delay <= 0) {
                delay = maxDelay;
                Move bestMove = findBestMove(mainBoard);
                mainBoard[bestMove.row][bestMove.col] = charPlayer;
                AnimatedObject* ao = new AnimatedObject("assets/spritesheets/animation_x.png", renderer, screenBoard->get_pBoard()[bestMove.row][bestMove.col].x, screenBoard->get_pBoard()[bestMove.row][bestMove.col].y, o_rects, 9, 5);
                ao->setDimensions(75, 75);
                ao->generateFrameRects(10, 50, 0, 49, 50);
                ao->setSoundEffectSource("assets/sounds/pencil_write.wav");
                ao->setPlaySoundEffect(true);
                animations.push_back(ao);
                playerTurn = !playerTurn;
            }
            /*Move bestMove = findBestMove(board);
            board[bestMove.row][bestMove.col] = player;
            sendBoard(board);
            playerTurn = true;
            */
        }

        int x, y;
        SDL_GetMouseState( &x, &y );
        restartButton->isTriggered(x,y);
        restartButton->update("assets/buttons/restart_button1.png", "assets/buttons/restart_button2.png");

        if(isThereWinner(mainBoard) || !isMovesLeft(mainBoard)) {
            //cout << "GameOver!\n";
            if(addScore) {
                if(evaluate(mainBoard) > 0) {
                    computerScore++;
                    string cs = ""+to_string(computerScore);
                    computerScoreText->setText(cs.c_str());
                    computerScoreText->setColor({0,0,0});
                } else if(evaluate(mainBoard) < 0) {
                    playerScore++;
                    string ps = ""+to_string(playerScore);
                    playerScoreText->setText(ps.c_str());
                    playerScoreText->setColor({0,0,0});
                }
                addScore = !addScore;
            }
            if(!restartButton->isShowing()) restartButton->setShow(true);
        }

	} else if(menu) {
        if(paper_back.x > 50) paper_back.x -= 5;
        if(paper_back.w < 700) paper_back.w += 5;

        if(animation_paper_left->getX()<=0) {
            screenBoard->startAnimation();
        }

        screenBoard->update();
        animation_paper_left->update();
        animation_paper_left->playAnimation();
        animation_paper_right->update();
        animation_paper_right->playAnimation();
        if(animation_paper_left->getAnimated() && animation_paper_left->getX() > 0) {
            animation_paper_left->setAnimated(false);
            animation_paper_right->setAnimated(false);
            if(!mapStarted) {
                mapStarted = true;
                Mix_PlayChannel(-1, map_effect, 0);
            }
        }
        if(animation_paper_left->getX() <= 0) {
            vt_animation->update({0,0,0});
            ct_animation->update({0,0,0});
            animation_paper_left->setSpeedX(0);
            if(toe_title->getY() <= 350) toe_title->setSpeedY(+20);
            else toe_title->setSpeedY(0);

            if(toe_title->getY() >= 350 && tac_title->getY() <= 214) {
                tac_title->setSpeedY(+21);
                if(!sound1played) {
                    Mix_PlayChannel( -1, title_effect, 0 );
                    sound1played = true;
                }
            } else {
                tac_title->setSpeedY(0);
            }

            if(tac_title->getY() >= 215 && tic_title->getY() <= 80) {
                tic_title->setSpeedY(+20);
                if(!sound2played) {
                    Mix_PlayChannel( -1, title_effect, 0 );
                    sound2played = true;
                }
            } else {
                tic_title->setSpeedY(0);
            }

            if(tic_title->getY() >= 80 && playButtonOpacity > 0 ) {
                playButtonOpacity-=5;
                if(!sound3played) {
                    Mix_PlayChannel( -1, title_effect, 0 );
                    sound3played = true;
                }
            }
            //cout << playButtonOpacity << " ";

        }

        tic_title->update();
        tac_title->update();
        toe_title->update();

        int x, y;
        SDL_GetMouseState( &x, &y );
        playButton->isTriggered(x,y);
        playButton->update("assets/buttons/play_button1.png", "assets/buttons/play_button2.png");



	} else if(difficulty) {
        int x, y;
        SDL_GetMouseState( &x, &y );
        easy->isTriggered(x,y);
        easy->update("assets/buttons/easy1.png", "assets/buttons/easy2.png");
        medium->isTriggered(x,y);
        medium->update("assets/buttons/medium1.png", "assets/buttons/medium2.png");
        hard->isTriggered(x,y);
        hard->update("assets/buttons/hard1.png", "assets/buttons/hard2.png");

	}

}

void Game::render()
{
	SDL_RenderClear(renderer);
	//animation_o->render();
	//animation_x->render();

	if(inGame) {

        SDL_SetRenderDrawColor(renderer, 248,236,194,255);
        SDL_RenderFillRect(renderer, &paper_back);

        /*
        SDL_SetRenderDrawColor(renderer, 185, 149, 102,255);
        SDL_RenderFillRect(renderer, &line1);
        SDL_RenderFillRect(renderer, &line2);
        SDL_RenderFillRect(renderer, &line3);
        SDL_RenderFillRect(renderer, &line4);
        */
        screenBoard->render();

        SDL_SetRenderDrawColor(renderer, 237, 195, 106, 255);

        animation_paper_left->render();
        animation_paper_right->render();



        for(int i = 0; i < animations.size(); i++) {
            animations[i]->render();
        }

        restartButton->render();
        drawText(playerText);
        drawText(playerScoreText);
        drawText(computerText);
        drawText(computerScoreText);
        //drawText(testText);

	} else if(menu) {
        SDL_SetRenderDrawColor(renderer, 248,236,194,255);
        SDL_RenderFillRect(renderer, &paper_back);

        SDL_SetRenderDrawColor(renderer, 237, 195, 106, 255);

        animation_paper_left->render();
        animation_paper_right->render();

        tic_title->render();
        toe_title->render();
        tac_title->render();

        if(animation_paper_left->getX() <= 0) {
            playButton->render();

            Uint8 a = (Uint8) playButtonOpacity;
            SDL_SetRenderDrawColor(renderer, 248,236,194, playButtonOpacity);
            SDL_RenderFillRect(renderer, &playButtonCoat);

            SDL_SetRenderDrawColor(renderer, 237, 195, 106, 255);

            //drawText(versionText);
            //drawText(creditsText);
            vt_animation->render();
            ct_animation->render();
        }
	} else if(difficulty) {
	    SDL_SetRenderDrawColor(renderer, 248,236,194,255);
        SDL_RenderFillRect(renderer, &paper_back);

        SDL_SetRenderDrawColor(renderer, 237, 195, 106, 255);

        animation_paper_left->render();
        animation_paper_right->render();

        easy->render();
        medium->render();
        hard->render();

        drawText(easyText);
        drawText(mediumText);
        drawText(hardText);
        drawText(comingSoon1);
        drawText(comingSoon2);

	}

	SDL_RenderPresent(renderer);

}

void Game::clean()
{

    Mix_FreeChunk( title_effect );
    title_effect = NULL;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	Mix_Quit();
}

void Game::drawText(MyText* text) {
    SDL_RenderCopy(renderer, text->getText(), NULL, text->getTextRect());
}

bool Game::pointInRect(SDL_Rect rect, int x, int y) {
    if( x >= rect.x && x <= rect.x+rect.w && y >= rect.y && y <= rect.y+rect.h ) return true;
    return false;
}
