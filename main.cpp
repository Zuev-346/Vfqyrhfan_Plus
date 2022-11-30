#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#define M_PI  3.14159265358979323846


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
std::vector<double> new_di_in_new_pos_func(double vec_1_x, double vec_1_y, double vec_1_z,
    double vector_nul_x, double vector_nul_y, double vector_nul_z)
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
        std::vector<double> temp = { 0, 0, 0, cos_cam_point, aspect_ratio, 0 };
        //0 == false
        return temp;
    }
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
            height * (1 - (dz / 2 * sqrt(3) / vector_nul_d + 1 / 2)), 1 };
        //1 == true
        return temp_3;
    }
    else
    {
        std::vector<double> temp_4 = { -10, -10, 0 };
        //0 == false
        return temp_4;
    };
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


double get_angle_cos_func(double dx, double dy, double dz, double d, double vector_nul_dx,
    double vector_nul_dy, double vector_nul_dz, double vector_nul_d)
    /*
    Косинус между двумя радиус-векторами
    */
{
    return scalar_func(dx, dy, dz, vector_nul_dx, vector_nul_dy, vector_nul_dz) / (d * vector_nul_d);
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
    Vector() {};
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
        std::vector<double> temp = from_relative_to_screen(dx, dy, dz, vector_nul.d,
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


class Scene
{
public:
    std::vector<std::vector<std::vector<int>>> map;
    Scene();
    ~Scene();
    void dest_block(std::vector<std::vector<int>>fat) {};
    void add_block(std::vector<std::vector<int>>fat, int color) {};
private:

};

Scene::Scene()
{
}

Scene::~Scene()
{
}



std::vector<std::vector<int>> cut(Scene scene, std::vector<std::vector<int>> order, Vector camera, int d, int h)
/*
выдаёт нужный порядок отрисовки кубов
order: массив из блоков с относительной координатой, хранится order_of_output.py
scene: массив из блоков, которые вокруг игрока
camera: камера игрока
d: диаметр отрисовки блоков
h: высота отрисовки блоков
return: массив в нужном порядке отрисовки блоков
*/
{
    std::vector<std::vector<int>> t_o;
    for (std::vector<int> item : order)
    {
        int x = item[0];
        int y = item[1];
        int z = item[2];
        x += int(camera.x + 0.5) - int(d / 2);
        y += int(camera.y + 0.5) - int(d / 2);
        z += int(camera.z + 0.5) - int(h / 2);
        if (scene.map[x][y][z])
        {
            std::vector<int>temp = { x, y, z };
            t_o.push_back(temp);
        }
    }
    return t_o;
}

std::vector<int> check_distance(Scene scene, Vector cam, std::vector<std::vector<int>> order, int grnd)
/*
проверяет нахождение блоков поблизости в кубе 3х3, возвращает 6 чисел в формате,
какую компоненту скорости нужно убить
return: [0,0,1,0,0,1]
*/
{
    std::vector<int> ret = { 0, 0, 0, 0, 0, 0 };
    for (std::vector<int>item : cut(scene, order, cam, 3, 3))
    {
        double dx = item[0] - cam.x;
        double dy = item[1] - cam.y;
        double dz = item[2] - cam.z;
        if (0 < dx && dx <= 1.45 && abs(dy) <= 0.5 && abs(dz) <= 0.5)
        {
            ret[0] += 1;
        }
        else if (0 < dy && dy <= 1.45 && abs(dx) <= 0.5 && abs(dz) <= 0.5)
        {
            ret[1] += 1;
        }
        else if (0 < dz && dz <= 1.45 && abs(dx) <= 0.5 && abs(dy) <= 0.5)
        {
            ret[2] += 1;
        }
        else if (0 < -dx && dx <= 1.45 && abs(dy) <= 0.5 && abs(dz) <= 0.5)
        {
            ret[3] += 1;
        }
        else if (0 < -dy && dy <= 1.45 && abs(dx) <= 0.5 && abs(dz) <= 0.5)
        {
            ret[4] += 1;
        }
        else if (0 < -dz && dz <= 1.45 && abs(dx) <= 0.5 && abs(dy) <= 0.5)
        {
            ret[5] += 1;
        }
        else if (abs(dx) <= 0.5 || abs(dy) <= 0.5 || abs(dz) <= 0.5)
        {
            if (sqrt((abs(dx) - 0.5) * (abs(dx) - 0.5) + (abs(dy) - 0.5) * (abs(dy) - 0.5)) <= 0.92 && abs(dz) <= 0.5)
            {
                if (dx > 0)
                    ret[0] += 1;
                else
                    ret[3] += 1;
                if (dy > 0)
                    ret[1] += 1;
                else
                    ret[4] += 1;
            };
            if (sqrt((abs(dx) - 0.5) * (abs(dx) - 0.5) + (abs(dz) - 0.5) * (abs(dz) - 0.5)) <= 0.92 && abs(dy) <= 0.5)
            {
                if (dx > 0)
                    ret[0] += 1;
                else
                    ret[3] += 1;
                if (dz > 0)
                    ret[2] += 1;
                else
                    ret[5] += 1;
            };

            if (sqrt((abs(dy) - 0.5) * (abs(dy) - 0.5) + (abs(dz) - 0.5) * (abs(dz) - 0.5)) <= 0.92 && abs(dx) <= 0.5)
            {
                if (dy > 0)
                    ret[1] += 1;
                else
                    ret[4] += 1;
                if (dz > 0)
                    ret[2] += 1;
                else
                    ret[5] += 1;
            }
        }
        else if (sqrt((abs(dy) - 0.5) * (abs(dy) - 0.5) + (abs(dx) - 0.5) * (abs(dx) - 0.5) + (abs(dz) - 0.5) * (abs(dz) - 0.5)) <= 0.87)
        {
            if (dx > 0)
                ret[0] += 1;
            else
                ret[3] += 1;
            if (dy > 0)
                ret[1] += 1;
            else
                ret[4] += 1;
            if (dz > 0)
                ret[2] += 1;
            else
                ret[5] += 1;
        };
    }

    if (cam.z <= grnd + 1.5)
        ret[5] += 1;
    if (cam.z >= 19)
        ret[2] += 1;
    return ret;
};


class Player
{
public:
    std::vector<double> r, v, a;
    std::vector<int>screen_size;
    Vector cam;
    double lng = 0;// longitude;
    double lat = 0;// latitude;
    int n = 8;
    int g = 9;
    //Что это? Дальность выбора кубика?
    double fly_mode = 1;
    double test_mode = .0f;
    int color = 4;

    double leg_force = 0.01;
    double stopper_acceleration = 0.19 * leg_force; // На сколько ед\с падают составляющие скорости
    double k = .001;  // Чувствительность мыши
    double speed_limit_max = 6 * leg_force;
    double speed_limit_min = 0.2 * leg_force;
    int ground = 9;
    // 0 == false

    std::vector<sf::Keyboard::Key> control_keys = { sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::D, sf::Keyboard::Space };
    std::vector<sf::Keyboard::Key> pressed_keys;
    //1 == true
    //TO-DO: move to vocabulary
    Player(){};
    Player(std::vector<int> screen_size0)
    {
        r = std::vector<double>{ 0, 0, 0 };
        screen_size = screen_size0;
    };
    Player(std::vector<double> point, std::vector<int> screen_size0)
        /*
            инициализирует игрока,
            longitude - долгота угла
            latitude - широта угла
            points: начальное положение
            g: ускорение свободного падения
            screen_size: размер экрана
        */
    {
        r = point;
        screen_size = screen_size0;
    }
    ~Player() {};

    Vector get_camera()
        /*
            return: возвращает вектор евклидовых координат из полярных координат
        */
    {
        cam.from_polar(r[0], r[1], r[2], lng, lat, 0.1);
        return cam;
    }
    //

    double update(sf::Event event, Scene scene, std::vector<std::vector<int>>fat)
        //TO-DO: find real "fat" type
        /*
            обновляет конфигурацию нажатых клавиш и перемещает угол взгляда игрока посредством измерения перемещения мыши,
            отвечает за обработку событий и распределяет задачи между методами по обработке событий
            event: пайгеймовское событие
            scene: хранит массив блоков и их цвета
            fat: хранит информацию о выделенном кубе
            return: 1
            //true == 1, если игрок поставил/убрал куб
        */
    {
        double const_1 = 0;
        //false == 0
        pressed_keys.clear();
        for (sf::Keyboard::Key item : control_keys)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                pressed_keys.push_back(item);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
                fly_mode = not fly_mode;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        {
            test_mode = not test_mode;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) and not fly_mode)
            jump();
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            scene.dest_block(fat);
            const_1 = 1;
            //true == 1
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            scene.add_block(fat, color);
            const_1 = 1;
            //true == 1
        }
        
        if (event.type == sf::Event::MouseWheelScrolled)
        {
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                change_color(static_cast<int>(event.mouseWheelScroll.delta));
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            double mx = event.mouseMove.x;
            double my = event.mouseMove.y;
            // x = - delta <= ось с пайгеймой не сходится
            //w, h = screen_size;
            double temp_x = -k * (mx - screen_size[0] / 2.f);
            double temp_y = k * (my - screen_size[1] / 2.f);

            sf::Mouse::setPosition(sf::Vector2i(static_cast<int>(screen_size[0] / 2.0f), static_cast<int>(screen_size[1] / 2.0f)));

            lng = std::fmod((lng + temp_x + M_PI), (M_PI * 2)) - M_PI;
            lat = (lat - temp_y);
            if (lat > M_PI/2)
                lat = M_PI/2;
            if (lat < -M_PI/2)
                lat = -M_PI/2;
        }
        //else if event.type == pygame.VIDEORESIZE
            //screen_size = (event.w, event.h);
        //TO-DO: Move this comment into cpp in future
        return const_1;
    }
    //

    int signnum_typical(double x) {
        if (x > 0.0) return 1;
        if (x < 0.0) return -1;
        return 0;
    }

    void move(std::vector<std::vector<int>> order, int ground, Scene scene)
        /*
            перемещает игрока посредством добавления вектору скорости ускорения, замедляет его при слишком большой скорости.
            order: соседние блоки в относительных координатах
            ground: уровень земли в мире
            scene: хранит массив блоков и их цвета
            return: None
        */
    {
        double v_horizontal = sqrt(v[0] * v[0] + v[1] * v[1]);
        if (v_horizontal > speed_limit_min)
        {
            a[1] = -v[1] / v_horizontal * stopper_acceleration;
            a[0] = -v[0] / v_horizontal * stopper_acceleration;
        }
        else
        {
            a[0] = 0;
            a[1] = 0;
        }
        if (abs(v[2]) > speed_limit_min)
            a[2] = -signnum_typical(v[2]) * stopper_acceleration * 2;
        else
            a[2] = 0;

        for (sf::Keyboard::Key key : pressed_keys)
        {
            if (key == sf::Keyboard::D)
            {
                a[0] += +leg_force * sin(lng);
                a[1] += -leg_force * cos(lng);
            }
            else if (key == sf::Keyboard::A)
            {
                a[0] += -leg_force * sin(lng);
                a[1] += +leg_force * cos(lng);
            }
            else if (key == sf::Keyboard::S)
            {
                a[0] += -leg_force * cos(lng);
                a[1] += -leg_force * sin(lng);
            }
            else if (key == sf::Keyboard::W)
            {
                a[0] += +leg_force * cos(lng);
                a[1] += +leg_force * sin(lng);
            }
            else if (key == sf::Keyboard::Space and fly_mode)
                /*if (pygame.key.get_mods() & pygame.KMOD_LSHIFT)
                    a[2] += -leg_force;
                else
                    a[2] += +leg_force;*/
                if (true)
                    a[2] += -leg_force;
            //TO-DO: Port it into cpp in future
        }
        if (not fly_mode)
            a[2] = g;

        if (v_horizontal > speed_limit_max)
        {
            v[0] *= speed_limit_max / v_horizontal;
            v[1] *= speed_limit_max / v_horizontal;
        }
        if ((abs(v[2]) > speed_limit_max) and fly_mode)
            v[2] *= speed_limit_max / abs(v[2]);
        if ((abs(v[2]) > 3 * speed_limit_max) and not fly_mode)
            v[2] *= 3 * speed_limit_max / abs(v[2]);
        if (abs(v[0]) <= speed_limit_min)
            v[0] = 0;
        if (abs(v[1]) <= speed_limit_min)
            v[1] = 0;
        if (abs(v[2]) <= speed_limit_min)
            v[2] = 0;

        check_tuk(order, ground, scene);
        std::vector<double> temp = { r[0] + v[0], r[1] + v[1], r[2] + v[2] };
        r = temp;
        std::vector<double> temp_1 = { v[0] + a[0], v[1] + a[1], v[2] + a[2] };
        v = temp_1;
    };
    //
    void check_tuk(std::vector<std::vector<int>> order, int ground, Scene scene)
        /*
        Проверяет нахождение поблизости блоков и изменяет вектор скорости для того, чтобы нельзя было к ним приближаться
        order: соседние блоки в относительных координатах
        ground: уровень земли в мире
        scene: хранит массив блоков и их цвета
        return: None
        */
    {
        std::vector<int> ret = check_distance(scene, cam, order, ground);
        if (ret[0] and v[0] >= 0)
        {
            v[0] = 0;
            r[0] -= 0.001;
        }
        if (ret[1] and v[1] >= 0)
        {
            v[1] = 0;
            r[1] -= 0.001;
        }
        if (ret[2] and v[2] >= 0)
        {
            v[2] = 0;
            r[2] -= 0.001;
        }
        if (ret[3] and v[0] <= 0)
        {
            v[0] = 0;
            r[0] += 0.001;
        }
        if (ret[4] and v[1] <= 0)
        {
            v[1] = 0;
            r[1] += 0.001;
        }
        if (ret[5] and v[2] <= 0)
        {
            v[2] = 0;
            r[2] += 0.001;
        }
    }
    //
    void jump()
        /*
        добавляет вектор скорости вверх при нулевой вертикальной компоненте скорости
        return: None
        */
    {
        if (abs(abs(v[2]) - 3.0E-3) < 1.0E-7)
            v[2] = speed_limit_max * 1.5;
    }
    //
    void change_color(int rotate)
        /*
        изменяет цвет в соответствии с поворотом колёсика мыши
        rotate: направление поворота колёсика (1- вперёд, -1 - назад)
        return: None
        */
    {
        if (rotate > 0)
            color += 1;
        else if(rotate < 0)
            color -= 1;
        if (color == 9)
            color = 1;
        else if(color == 0)
            color = 8;
    }
    //
};

class Game
{
public:
	const int FPS = 60;
	const double gravity = -0.003f;
	const int ground = 9;
	std::vector<int> screen_size;
	Player Steve;
	Game(std::vector<int> input)
	{
		std::cout << "You're welcome!" << std::endl;
		if (input.size() == 2)
		{
			screen_size = input;
            Player Steve(screen_size);
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
		float precision = 0;
		float recLevel = 0;
		while (window.isOpen())
		{
			
			
			sf::CircleShape gunsight;
			gunsight.setRadius(5);
			gunsight.setOutlineColor(sf::Color::Red);
			gunsight.setFillColor(sf::Color::Transparent);
			gunsight.setOutlineThickness(1.5f);
			gunsight.setPosition(static_cast<int>(screen_size[0] / 2.0f) - gunsight.getRadius(), static_cast<int>(screen_size[1] / 2.0f) - gunsight.getRadius());


			sf::Font font;
			font.loadFromFile("fonts/arial.ttf");


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
			sf::Event evnt;
			while (window.pollEvent(evnt))
			{

			}
			if (evnt.type == sf::Event::MouseWheelScrolled)
			{
				if (evnt.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
					std::cout << "wheel type: vertical" << std::endl;
				else if (evnt.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
					std::cout << "wheel type: horizontal" << std::endl;
				else
					std::cout << "wheel type: unknown" << std::endl;
				std::cout << "wheel movement: " << evnt.mouseWheelScroll.delta << std::endl;
				std::cout << "mouse x: " << evnt.mouseWheelScroll.x << std::endl;
				std::cout << "mouse y: " << evnt.mouseWheelScroll.y << std::endl;
			}
			if (evnt.type == sf::Event::MouseMoved)
			{
				std::cout << "new mouse x: " << evnt.mouseMove.x << std::endl;
				std::cout << "new mouse y: " << evnt.mouseMove.y << std::endl;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				
			}
			std::cout << "precision: " << precision << std::endl;
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

int main()
{
	
	const int WIDTH = 1400;
	const int HEIGHT = 750;
	std::vector<int> screen_size = { 1400, 750 };
	Game Minecraft(screen_size);
	Minecraft.Loop();
	return 0;
}