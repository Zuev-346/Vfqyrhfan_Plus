

//==============================================================================
/*class drawable_object
{
public:
    sf::Sprite
};*/

struct VectorPolar
{
    // longitude - долгота угла(далее местами xy)
    // latitude - широта угла(далее местами xz)
    VectorPolar(double d, double lng, double lat) : d{ d }, longtitude{ h }, latitude{ v } {}

    double longtitude = .0;
    double latitude = .0;
    double d = .0;
};

struct Vector3d
{
    Vector3d(double x, double y, double z) : x{ x }, y{ y }, z{ z } {}
    double x = .0;
    double y = .0;
    double z = .0;
};

struct Vector2d
{
    Vector2d(double x, double y) : x{ x }, y{ y } {}
    double x = .0;
    double y = .0;
};

struct VectorCam
{
    VectorCam(Vector3d xyz, Vector3d dxdydz, VectorPolar dll) :
        xyz{ xyz }, dxdydz{ dxdydz }, dll{ dll } {}
    Vector3d dxdydz = { .0, .0, .0 };
    Vector3d xyz = { .0, .0, .0 };
    VectorPolar dll = { .0, .0, .0 };
    // dll.longtitude_sin, dll.longtitude_cos, dll.latitude_sin, dll.latitude_cos
    std::vector<double> trigonometry_array = { 0, 1, 0, 1 };

    VectorCam from_polar(Vector3d xyz, double lng, double lat, double r)
        /*
        задаёт относительные координаты из полярных координат
        return : vector с посчитанными относительными координатами
        */
    {
        VectorCam vector = VectorCam(xyz, Vector3d(0, 0, 0), dll);
        vector.set_coords_di_from_d();
        return vector;
    };

    void set_coords_di_from_d()
        /*задаёт относительные координаты из полярных координат
        return : None*/
    {
        trigonometry_array[0] = sin(dll.longtitude);
        trigonometry_array[1] = cos(dll.longtitude);
        trigonometry_array[2] = sin(dll.latitude);
        trigonometry_array[3] = cos(dll.latitude);
        dxdydz.x = dll.d * trigonometry_array[1] * trigonometry_array[3];
        dxdydz.y = dll.d * trigonometry_array[0] * trigonometry_array[3];
        dxdydz.z = dll.d * trigonometry_array[2];
    };

    void new_di_in_new_pos(Vector3d vector_nul)
        /*
        задаёт относительные координаты между двумя точками
        vector_nul : второй вектор
        return : None
        */
    {
        dxdydz.x = -vector_nul.x + xyz.x;
        dxdydz.y = -vector_nul.y + xyz.y;
        dxdydz.z = -vector_nul.z + xyz.z;
    };


    Vector2d from_world_to_screen(Vector3d vector_nul)
        /*
        выдаёт координаты на экране из абсолютных координат в мире
        vector_nul : точка, которую требуется отрисовать
        return : x, y точки
        */
    {
        ///VectorCam.new_di_in_new_pos(vector_nul)
        //return from_relative_to_screen(self.dx, self.dy, self.dz, vector_nul.d, vector_nul.trigonometry_array)
    };

    Vector2d from_world_to_screen()//self, vector_nul)
        /*
        выдаёт координаты на экране из абсолютных координат в мире
        vector_nul: точка, которую требуется отрисовать
        return: x, y точки
        */
    {
        //self.new_di_in_new_pos(vector_nul)
            //return from_relative_to_screen(self.dx, self.dy, self.dz, vector_nul.d,
                //vector_nul.trigonometry_array)
    };

    Vector2d from_relative_to_screen()//self_dx, self_dy, self_dz, vector_nul_d, trigonometry, screen_size)
        /*
            Функция проецирования точки на камеру(точка в относительных координатах)
        */
    {
        //vector_nul_dx, vector_nul_dy, vector_nul_dz = set_coords_di_from_d(vector_nul_d, trigonometry)

            //self_d = set_coords_d_from_di_func(self_dx, self_dy, self_dz)

            //dx, dy, dz, cos_cam_point, aspect_ratio, condition = find_projected_vector(self_dx, self_dy, self_dz, self_d,
                //vector_nul_dx, vector_nul_dy,
                //vector_nul_dz,
                //vector_nul_d)
            //if condition:
        //dx, dz = transformation_to_screen(dx, dy, dz, trigonometry)

//			width, height = screen_size
//			return width * (dx / 2 / vector_nul_d + 1 / 2), height* (1 - (dz / 2 * sqrt(3) / vector_nul_d + 1 / 2))
    //		else:
        //return -10, -10
    };


    /*
    void set_coords_d_from_di(self) :
    */
    устанавливает d посредством di
        * /
        self.d = np.sqrt(self.dx * *2 + self.dy * *2 + self.dz * *2)

        void scalar(self, vector_nul) :
        */
        считает скалярное произведение между веторами
        vector_nul :
    return : scalar
        * /
        return self.dx * vector_nul.dx + self.dy * vector_nul.dy + self.dz * vector_nul.dz

        void get_angle_cos(self, vector_nul) :
        */
        выдаёт косинус между векторами
        vector_nul : второй вектор
        return : cos между векторами
        * /
        return self.scalar(vector_nul) / (self.d * vector_nul.d)

        void rotate_vector_z(self, fi_xy = 0.0) :
        */
        вращает матрицу векторна на угол fi_xy вокруг оси z
        fi_xy : угол поворота
        return : None
        * /
        self.dx, self.dy = r_v_z(self.dx, self.dy, fi_xy)

        void rotate_vector_y(self, fi_zx = 0.0) :
        */
        вращает матрицу векторна на угол fi_xz вокруг оси y
        fi_zx : угол поворота
        return : None
        * /
        self.dx, self.dz = r_v_y(self.dx, self.dz, fi_zx)

        void rotate_vector_x(self, fi_yz = 0.0) :
        */
        вращает матрицу вектора на на угол fi_yz вокруг оси x
        fi_yz : угол поворота
        return : None
        * /
        self.dz, self.dy = r_v_x(self.dz, self.dy, fi_yz)
        * /

        void print_all()
        /*
        отладочная функция, выводит все основные данные вектора
        */
    {
        std::cout << "d = " << dll.d << std::endl;
        std::cout << "x = " << xyz.x << std::endl;
        std::cout << "y = " << xyz.y << std::endl;
        std::cout << "z = " << xyz.z << std::endl;
        std::cout << "dx = " << dxdydz.x << std::endl;
        std::cout << "dy = " << dxdydz.y << std::endl;
        std::cout << "dz = " << dxdydz.z << std::endl;
        std::cout << "dll.longtitude = " << dll.longtitude << std::endl;
        std::cout << "dll.longtitude = " << dll.latitude << std::endl;
    };
};

class Character
{
public:
    std::vector<double> coords = { .0, .0, .0 };
    std::vector<double> cam = { .0, .0 };
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
//==================================================================================================================================





while (window.pollEvent(evnt))
{

    constr = steve.update(evnt, scene, rasterizer.fat);
    if (evnt.type == sf::Event::Closed)
    {
        window.close();
    }
    /*
    if (evnt.type == sf::Event::MouseWheelScrolled)
    {
        if (evnt.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            //std::cout << "wheel type: vertical" << std::endl;
        else if (evnt.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
            //std::cout << "wheel type: horizontal" << std::endl;
        else
            //std::cout << "wheel type: unknown" << std::endl;
        //std::cout << "wheel movement: " << evnt.mouseWheelScroll.delta << std::endl;
        //std::cout << "mouse x: " << evnt.mouseWheelScroll.x << std::endl;
        //std::cout << "mouse y: " << evnt.mouseWheelScroll.y << std::endl;
    }
    if (evnt.type == sf::Event::MouseMoved)
    {
        //std::cout << "new mouse x: " << evnt.mouseMove.x << std::endl;
        //std::cout << "new mouse y: " << evnt.mouseMove.y << std::endl;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {

    }
    */
};
//==================================================================================================================================
