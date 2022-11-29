#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

class Character
{
public:
	Character();
	~Character();

private:

};

Character::Character()
{
}

Character::~Character()
{
}

class Game
{
public:
	const int FPS = 60;
	const double gravity = -0.003f;
	const int ground = 9;
	std::vector<int> screen_size;
	Character Player;
	Game(std::vector<int> input)
	{
		std::cout << "You're welcome!" << std::endl;
		if (input.size() == 2)
		{
			screen_size = input;
		}
		else
		{
			std::cout << "Invalid format of window's size" << std::endl;
			//std::cout << "input vector: " << input << std::endl;
		}
	};
	~Game()
	{
		std::cout << "Thanks!" << std::endl;
	};
	void Loop()
	{
		sf::RenderWindow window(sf::VideoMode(screen_size[0], screen_size[1]), "Vfqyrhfan");
		window.setFramerateLimit(FPS);
		/*
		while running:
			for event in pygame.event.get():
				constr = player.update(event, scene, rasterizer.fat)
			player.move(order_of_tuk.order, ground, scene)

			rasterizer.draw(screen, scene, //player_get_camera = player.get_camera()//, constr)
			constr = False
			gui(player.color)
			pygame.display.update()

		*/
		while (window.isOpen())
		{
			sf::Event evnt;
			while (window.pollEvent(evnt))
			{

			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
			}
			sf::CircleShape gunsight;
			gunsight.setRadius(5);
			gunsight.setOutlineColor(sf::Color::Red);
			gunsight.setFillColor(sf::Color::Transparent);
			gunsight.setOutlineThickness(1.5f);
			gunsight.setPosition(static_cast<int>(screen_size[0] / 2.0f) - gunsight.getRadius(), static_cast<int>(screen_size[1] / 2.0f) - gunsight.getRadius());


			sf::Font font;
			font.loadFromFile("fonts/arial.ttf");

			float precision = 0;
			float recLevel = 0;
			sf::Text zoomText, precText;
			zoomText.setFont(font);
			precText.setFont(font);
			zoomText.setFillColor(sf::Color::Black);
			precText.setFillColor(sf::Color::Black);
			zoomText.setCharacterSize(24);
			precText.setCharacterSize(24);
			zoomText.setString("Zoom: " + std::to_string(pow(8, recLevel - 1)));
			precText.setString("Max. Iterations: " + std::to_string(precision));
			precText.setPosition(sf::Vector2f(0, 32));
			sf::Color const BACKGROUND = sf::Color(180, 180, 180, 0);
			// TO-DO: move to vocabulary
			window.clear(BACKGROUND);
			window.draw(zoomText);
			window.draw(precText);
			window.draw(gunsight);
			window.display();
		}
		//void GUI(sf::Color color){
			/*
			graphic user interface
			отвечает за вывод информации для тестера на экран, за кружочек в центре экрана
			color : цвет в формате от 1 до 8
			return : None
			*/
			//w, h = self.screen.get_clip().size
			//circle(self.screen, get_color(color), (w / 2, h / 2), 6, 2)
			//coords(self.screen, self.player, self.clock.get_fps())

		//};
	}

private:

};

// New begining
double scalar_func(double dx, double dy, double dz, double vector_nul_dx, double vector_nul_dy, double vector_nul_dz)
/*
Скалярное произведение двух векторов по составляющим
*/
{
	return dx * vector_nul_dx + dy * vector_nul_dy + dz * vector_nul_dz;
};
double set_coords_d_from_di_func(double dx, double dy, double dz)
/*
Определение длины вектора
*/
{
	return sqrt(dx * dx + dy * dy + dz * dz);
};
std::vector<double> new_di_in_new_pos_func(double vec_1_x, double vec_1_y, double vec_1_z, double vector_nul_x, double vector_nul_y, double vector_nul_z)
/*
Векторная разность
*/
{

	double dx = -vector_nul_x + vec_1_x;
	double dy = -vector_nul_y + vec_1_y;
	double dz = -vector_nul_z + vec_1_z;
	std::vector<double> temp = { dx, dy, dz };
	return temp;

}
std::vector<double> from_world_to_screen(double self_x, double self_y, double self_z, double vector_nul_x, double vector_nul_y, double vector_nul_z,
	double vector_nul_d, std::vector<double> trigonometry, std::vector<double> screen_size)
	/*
	Функция проецирования точки на камеру(точка в абсолютных координатах)
	*/
{
	std::vector<double> temp = new_di_in_new_pos_func(self_x, self_y, self_z, vector_nul_x, vector_nul_y, vector_nul_z);
	double self_dx = temp[0];
	double self_dy = temp[1];
	double self_dz = temp[2];
	return from_relative_to_screen(self_dx, self_dy, self_dz, vector_nul_d, trigonometry, screen_size);

}
std::vector<double> from_relative_to_screen(double self_dx, double self_dy, double self_dz, double vector_nul_d, std::vector<double> trigonometry,
	std::vector<double> screen_size)
	/*
	Функция проецирования точки на камеру(точка в относительных координатах)
	*/
{
	std::vector<double> temp = set_coords_di_from_d(vector_nul_d, trigonometry);
	double vector_nul_dx = temp[0];
	double vector_nul_dy = temp[1];
	double vector_nul_dz = temp[2];
	double self_d = set_coords_d_from_di_func(self_dx, self_dy, self_dz);
	std::vector<double> temp_1 = find_projected_vector(self_dx, self_dy, self_dz, self_d,
		vector_nul_dx, vector_nul_dy, vector_nul_dz, vector_nul_d);
	double dx = temp_1[0];
	double dy = temp_1[1];
	double dz = temp_1[2];
	double cos_cam_point = temp_1[3];
	double aspect_ratio = temp_1[4];
	double condition = temp_1[5];

	if (condition)
	{
		std::vector<double> temp_2 = transformation_to_screen(dx, dy, dz, trigonometry);
		dx = temp_2[0];
		dz = temp_2[1];
		double width = screen_size[0];
		double height = screen_size[1];
		std::vector<double> temp_3 = { width * (dx / 2 / vector_nul_d + 1 / 2),
			height * (1 - (dz / 2 * sqrt(3) / vector_nul_d + 1 / 2)), 1};
		//1 == true
		return temp_3;
	}
	else
	{
		std::vector<double> temp_4 = { -10, -10, 0};
		//0 == false
		return temp_4;
	};
}

std::vector<double> transformation_to_screen(double dx, double dy, double dz, std::vector<double> trigonometry)
/*
Преобразование координат в вид, соответствующий отрисовке пайгейма
*/

{
	std::vector<double> temp = { dx * trigonometry[1] + dy * trigonometry[0], -dx * trigonometry[0] + dy * trigonometry[1] };
	temp.push_back(-temp[0] * trigonometry[2] + dz * trigonometry[3]);
	std::vector<double> temp_1 = { (-1.0f * temp[1]), (temp[2]) };
	return temp_1;
}

std::vector<double> set_coords_di_from_d(double vector_nul_d, std::vector<double> trigonometry)
/*
Переход из полярных координат
*/
{
	std::vector<double> temp = { vector_nul_d * trigonometry[1] * trigonometry[3],
		vector_nul_d * trigonometry[0] * trigonometry[3], vector_nul_d * trigonometry[2] };
	return temp;
}

std::vector<double> find_projected_vector(double self_dx, double self_dy, double self_dz, double self_d, double vector_nul_dx,
	double vector_nul_dy, double vector_nul_dz, double vector_nul_d)
	/*
	Укорачивание вектора до камеры и проверка видимости
	*/
{
	double aspect_ratio = (self_dx * vector_nul_dx + self_dy * vector_nul_dy + self_dz * vector_nul_dz) / (vector_nul_d);
	double cos_cam_point = aspect_ratio / self_d;
	aspect_ratio /= vector_nul_d;
	double angle_of_view = 0.3;
	//TO-DO: move to vocabulary
	if (cos_cam_point > angle_of_view)
	{
		double dx = self_dx / aspect_ratio - vector_nul_dx;
		double dy = self_dy / aspect_ratio - vector_nul_dy;
		double dz = self_dz / aspect_ratio - vector_nul_dz;
		std::vector<double> temp = { dx, dy, dz, cos_cam_point, aspect_ratio, 1 };
		//1==true
		return temp;
	}
	else
	{
		std::vector<double> temp = { 0, 0, 0, cos_cam_point, aspect_ratio, 0};
		//0 == false
		return temp;
	}
}

double get_angle_cos_func(double dx, double dy, double dz, double d, double vector_nul_dx,
	double vector_nul_dy, double vector_nul_dz, double vector_nul_d)
	/*
	Косинус между двумя радиус-векторами
	*/
{
	return scalar_func(dx, dy, dz, vector_nul_dx, vector_nul_dy, vector_nul_dz) / (d * vector_nul_d);
}

std::vector<double> new_di_in_new_pos_func(double self_x, double self_y, double self_z,
	double vector_nul_x, double vector_nul_y, double vector_nul_z)
	/*
	Векторное вычитаение
	*/
{
	double self_dx = -vector_nul_x + self_x;
	double self_dy = -vector_nul_y + self_y;
	double self_dz = -vector_nul_z + self_z;
	std::vector<double> temp = { self_dx, self_dy, self_dz };
	return temp;
}



/*
Ниже находятся матрицы поворота относительно указанных осей в правых тройках
*/

std::vector<double> r_v_z(double x, double y, double fi_xy)
//double fi_xy = 0.0
{
	std::vector<double> temp = { x * cos(fi_xy) - y * sin(fi_xy), x * sin(fi_xy) + y * cos(fi_xy) };
	return temp;
}

std::vector<double> r_v_y(double x, double z, double fi_zx)
//fi_zx = 0.0
{
	std::vector<double> temp = { x * cos(fi_zx) + z * sin(fi_zx), -x * sin(fi_zx) + z * cos(fi_zx) };
	return temp;
}

std::vector<double> r_v_x(double z, double y, double fi_yz)
//fi_yz = 0.0
{
	std::vector<double> temp = { y * sin(fi_yz) + z * cos(fi_yz), y * cos(fi_yz) - z * sin(fi_yz) };
	return temp;
}

class Vector
{
public:
	double d;
	double x;
	double y;
	double z;
	double dx;
	double dy;
	double dz;
	double an_xy;
	double an_xz;
	std::vector<double>trigonometry_array;
	std::vector<double>screen_size;
	Vector(double x0, double y0, double z0, double d0, double dx0, double dy0, double dz0,
		double an_xy0, double an_xz0, std::vector<double> screen_size0)
		/*
		инициализирует объект класса вектор
		*/
	{
		d = d0;
		x = x0;
		y = y0;
		z = z0;
		dx = dx0;
		dy = dy0;
		dz = dz0;
		an_xy = an_xy0;
		an_xz = an_xz0;
		screen_size = screen_size0;
		// an_xy_sin, an_xy_cos, an_xz_sin, an_xz_cos
		std::vector<double>trigonometry_array = { 0, 1, 0, 1 };
	}

	Vector from_polar(double x, double y, double z, double lng, double lat, double r)
		/*
		задаёт относительные координаты из полярных координат
		return: vector с посчитанными относительными координатами
		*/
	{
		Vector vector(x, y, z, r, 0, 0, 0, lng, lat, screen_size);
		vector.set_coords_di_from_d_method();
		return vector;
	}

	void set_coords_di_from_d_method()
		/*
		задаёт относительные координаты из полярных координат
		return:None
		*/
	{
		trigonometry_array.push_back(sin(an_xy));
		trigonometry_array.push_back(cos(an_xy));
		trigonometry_array.push_back(sin(an_xz));
		trigonometry_array.push_back(cos(an_xz));
		std::vector<double> temp = set_coords_di_from_d(d, trigonometry_array);
		dx = temp[0];
		dy = temp[1];
		dz = temp[2];
	}

	void new_di_in_new_pos_method(Vector vector_nul)
		/*
		задаёт относительные координаты между двумя точками
		vector_nul: второй вектор
		return: None
		*/
	{
		dx = -vector_nul.x + x;
		dy = -vector_nul.y + y;
		dz = -vector_nul.z + z;
	}

	std::vector<double> from_world_to_screen(Vector vector_nul)
		/*
		выдаёт координаты на экране из абсолютных координат в мире
		vector_nul: точка, которую требуется отрисовать
		return: x, y точки
		*/
	{
		new_di_in_new_pos_method(vector_nul);
		std::vector<double> temp =  from_relative_to_screen(dx, dy, dz, vector_nul.d,
			vector_nul.trigonometry_array, screen_size);
		return temp;
	}

	void set_coords_d_from_di()
		/*
		устанавливает d посредством di
		*/
	{
		d = sqrt(dx * dx + dy * dy + dz * dz);
	}

	double scalar(Vector vector_nul)
		/*
		считает скалярное произведение между веторами
		vector_nul:
		return: scalar
		*/
	{
		return dx * vector_nul.dx + dy * vector_nul.dy + dz * vector_nul.dz;
	}

	double get_angle_cos(Vector vector_nul)
		/*
		выдаёт косинус между векторами
		vector_nul:второй вектор
		return: cos между векторами
		*/
	{
		return scalar(vector_nul) / (d * vector_nul.d);
	}

	void rotate_vector_z(double fi_xy)
		//fi_xy = 0.0
		/*
		вращает матрицу векторна на угол fi_xy вокруг оси z
		fi_xy: угол поворота
		return: None
		*/
	{
		std::vector<double> temp = r_v_z(dx, dy, fi_xy);
		dx = temp[0]; dy = temp[1];
	}

	void rotate_vector_y(double fi_zx)
		//fi_zx = 0.0
		/*
		вращает матрицу векторна на угол fi_xz вокруг оси y
		fi_zx: угол поворота
		return: None
		*/
	{
		std::vector<double> temp = r_v_y(dx, dz, fi_zx);
		dx = temp[0]; dz = temp[1];
	}

	void rotate_vector_x(double fi_yz)
		//fi_yz = 0.0
		/*
		вращает матрицу вектора на на угол fi_yz вокруг оси x
		fi_yz: угол поворота
		return: None
		*/
	{
		std::vector<double> temp = r_v_x(dz, dy, fi_yz);
		dz = temp[0]; dy = temp[1];
	}
};

int main()
{
	
	const int WIDTH = 1400;
	const int HEIGHT = 750;
	std::vector<int> screen_size = { 1400, 750 };
	Game Minecraft(screen_size);
	Minecraft.Loop();
	return 0;
}