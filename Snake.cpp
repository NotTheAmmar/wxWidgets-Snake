#include<wx/wx.h>
#include<vector>

using namespace std;

class SnakeApp : public wxApp {
private:
	enum Direction {UP, RIGHT, DOWN, LEFT};
	const int WIDTH = 800, HEIGHT = 600, SIZE = 20;
	Direction direction = DOWN;
	vector<wxPoint> snake;
	wxFrame *win;
	wxTimer *timer;
	wxPoint food = wxPoint();

	void onKeyDownEVT(wxKeyEvent& evt) {
		switch (evt.GetKeyCode()) {
			case WXK_UP:
				if (direction != DOWN) direction = UP;
				break;
			case WXK_RIGHT:
				if (direction != LEFT) direction = RIGHT;
				break;
			case WXK_DOWN:
				if (direction != UP) direction = DOWN;
				break;
			case WXK_LEFT:
				if (direction != RIGHT) direction = LEFT;
				break;
			default:
				break;
		}
	}
	
	void OnPaintEVT(wxPaintEvent& evt) {
		wxPaintDC dc(win);

		dc.SetBrush(wxBrush(wxColor(255, 0, 0)));
		dc.DrawRectangle(wxRect(food.x, food.y, SIZE, SIZE));

		dc.SetBrush(wxBrush(wxColor(0, 255, 0)));
		for (wxPoint &part : snake) {
			dc.DrawRectangle(part.x, part.y, SIZE, SIZE);
		}
	}

	void generateFood() {
		srand(time(0));
		food.x = (rand() % ((WIDTH - SIZE + 1) / SIZE)) * SIZE;
		food.y = (rand() % ((HEIGHT - SIZE + 1) / SIZE)) * SIZE;

		for (wxPoint& part : snake) {
			if (food.x == part.x && food.y == part.y) {
				generateFood();
				break;
			}
		}
	}

	void OnTimerEVT(wxTimerEvent& evt) {
		moveSnake();
		win->Refresh();
	}

	void moveSnake() {
		wxPoint head = snake.front();

		switch (direction) {
			case UP:
				head.y -= SIZE;
				break;
			case RIGHT:
				head.x += SIZE;
				break;
			case DOWN:
				head.y += SIZE;
				break;
			case LEFT:
				head.x -= SIZE;
				break;
		}

		snake.insert(snake.begin(), head);
		if (head.x == food.x && head.y == food.y) {
			generateFood();
		} else if (checkCollisions(head)) {
			timer->Stop();

			wxMessageDialog* msgDialog = new wxMessageDialog(win, "Game Over");
			msgDialog->ShowModal();
			win->Close();
		} else {
			snake.pop_back();
		}
	}

	bool checkCollisions(wxPoint &head) {
		if (head.x < 0 || head.x >= WIDTH) {
			return true;
		} else if (head.y < 0 || head.y >= HEIGHT) {
			return true;
		}
		
		for (int i = 1; i < snake.size(); i++) {
			if (head.x == snake.at(i).x && head.y == snake.at(i).y) {
				return true;
			}
		}

		return false;
	}
public:
	bool OnInit() {
		win = new wxFrame(NULL, wxID_ANY, "Snake Game", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX);
		win->SetClientSize(WIDTH, HEIGHT);
		win->SetBackgroundColour(wxColour(0, 0, 0));
		win->Center();
		win->Bind(wxEVT_KEY_DOWN, &SnakeApp::onKeyDownEVT, this);
		win->Bind(wxEVT_PAINT, &SnakeApp::OnPaintEVT, this);

		generateFood();

		snake.push_back(wxPoint(0, 0));

		timer = new wxTimer(win);
		win->Bind(wxEVT_TIMER, &SnakeApp::OnTimerEVT, this);
		timer->Start(125);

		win->Show();
		return true;
	};
};

wxIMPLEMENT_APP(SnakeApp);
