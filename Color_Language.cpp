#define _CRT_SECURE_NO_WARNINGS

#include <bangtal>
#include <cstdlib>
#include <ctime>

using namespace bangtal;

TimerPtr timer_finish;
ScenePtr scene = Scene::create("N_Back Game", "Images/background.png");
ObjectPtr restart = 0;
ObjectPtr end = 0;
ObjectPtr notice = 0;
ObjectPtr yes_play_song = 0;
ObjectPtr no_play_song = 0;
ObjectPtr placement[7][7] = { 0, };
SoundPtr song = 0;

int arr_20_1[21][3] = { 0, }, arr_20_2[21][3] = { 0, }, result_true[21] = { 0, }, result_your[45] = { 0, }, result[21] = { 0, };
int trial = 20; int count = 0;
int choose_num = 0; int num_right = 0;

void song_object() {
	yes_play_song = Object::create("Images/Play_Song_Yes.png", scene, 800, 700); yes_play_song->setScale(0.5);
	yes_play_song->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		song->play(true);
		yes_play_song->hide();
		no_play_song->show();
		return true;
		});

	no_play_song = Object::create("Images/Play_Song_No.png", scene, 800, 700); no_play_song->setScale(0.5);
	no_play_song->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		song->stop();
		no_play_song->hide();
		yes_play_song->show();
		return true;
		});

	yes_play_song->hide();
}

void makearray_start() {
	printf("\n\n\n");
	printf("======================================================================");
	printf("\n\n\n");

	srand((unsigned int)time(NULL));
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			arr_20_1[i][j] = rand() % 7;
			arr_20_2[i][j] = rand() % 7;
			if (arr_20_1[i][j] == arr_20_2[i][j])
			{
				arr_20_2[i][j] = arr_20_2[i][j] + 1;
				arr_20_2[i][j] = arr_20_2[i][j] % 7;
			}
		}
		if ((rand() % 3) == 0)
		{
			arr_20_1[i][0] = arr_20_2[i][1];
		}
	}
}


void makeresult() {
	printf("\n\n");
	num_right = 0;

	for (int i = 0; i < 20; i++)
	{
		if (arr_20_1[i][0] == arr_20_2[i][1])
		{
			result_true[i] = 1;
		}
		else
		{
			result_true[i] = 2;
		}

		if (result_true[i] == result_your[i * 2 ])
		{
			printf("O ");
			num_right++;
		}
		else
		{
			printf("X ");
		}
	}

	printf("\n");
	for (int i = 0; i < 20; i++)
	{
		printf("%d ", result_true[i]);
	}
	printf("\n");
	for (int i = 0; i < 20; i++)
	{
		printf("%d ", result_your[i * 2]);
	}

	char message_in[100];
	sprintf(message_in, "Your score is %d out of 20 !!!", num_right);
	showMessage(message_in);

}

void reset() {
	trial = 20;
	count = 0;
}

void init_game()
{
	scene->setOnKeyboardCallback([&](ScenePtr scene, KeyCode key, bool pressed)->bool {
		if (count < 40)
		{
			if (key == KeyCode::KEY_LEFT_ARROW)
			{
				result_your[count] = 1; //전환 show, hide
			}
			if (key == KeyCode::KEY_RIGHT_ARROW)
			{
				result_your[count] = 2; //전환 show, hide
			}
			printf(" %d\n", count);
			int input_inst = count / 2 + 1;
			if (count % 2 == 0)
			{
				if (input_inst >= 1)
				{
					placement[arr_20_1[input_inst-1][0]][arr_20_1[input_inst-1][1]]->hide();
					placement[arr_20_2[input_inst-1][0]][arr_20_2[input_inst-1][1]]->hide();
				}

				placement[arr_20_1[input_inst][0]][arr_20_1[input_inst][1]]->show();
				placement[arr_20_1[input_inst][0]][arr_20_1[input_inst][1]]->locate(scene, 200, 300);
				placement[arr_20_2[input_inst][0]][arr_20_2[input_inst][1]]->show();
				placement[arr_20_2[input_inst][0]][arr_20_2[input_inst][1]]->locate(scene, 400, 300);
			}
		}
		
		count++;

		if (count == 39)
		{
			timer_finish->set(0.5); timer_finish->start();
		}
		return true;
	});

	timer_finish = Timer::create(0.5);
	timer_finish->setOnTimerCallback([&](auto)->bool {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 7; j++)
			{
				placement[i][j]->hide();
			}
		};
		makeresult();
		return true;
	});

	restart = Object::create("Images/start.png", scene, 750, 150);
	restart->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		reset();
		makearray_start();
		placement[arr_20_1[0][0]][arr_20_1[0][1]]->show();
		placement[arr_20_1[0][0]][arr_20_1[0][1]]->locate(scene, 200, 300);
		placement[arr_20_2[0][0]][arr_20_2[0][1]]->show();
		placement[arr_20_2[0][0]][arr_20_2[0][1]]->locate(scene, 400, 300);

		return true;
		});

	end = Object::create("Images/end.png", scene, 750, 100);;
	end->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		endGame();
		return true;
		});

	startGame(scene);
}

int main() {
	char path[50];
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++)
		{
			sprintf(path, "Images/%d_%d.png", i, j);
			placement[i][j] = Object::create(path, scene, 200, 300);
			placement[i][j]->setScale(0.75);
			placement[i][j]->hide();
		}
	};

	song = Sound::create("Sound/x-mas.mp3"); song->play(true);
	song_object();

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	init_game();

	return 0;
}