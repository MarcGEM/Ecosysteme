#include "graph.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
ifstream fichier("ocean.txt", ios::in);  // on ouvre le fichier en lecture

            int num;
            int index;
            int posx;
            int posy;
            float population;
            string image;
            int ponderer;
        if(fichier)  // si l'ouverture a réussi

        {

            fichier>>num;


            cout<<num<<endl;
            for(int i=0;i<num;i++)
            {
                fichier>>index>>population>>posx>>posy>>image;

                add_interfaced_vertex(i, population, posx, posy, image);
            }

            int tab[num][num];
            int f=0;

            for (int m=0;m<num;m++)
                for(int n=0;n<num;n++)
            {
                fichier>>ponderer;
                cout<<ponderer<<endl;
                if (ponderer != 0)
                {
                   add_interfaced_edge(f,m,n,ponderer);
                   f=f+1;
                }

            }

                fichier.close();  // on ferme le fichier

        }

        else  // sinon

                cerr << "Impossible d'ouvrir le fichier !" << endl;





    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    //add_interfaced_edge(0, 1, 2, 50.0);

}
void Graph::create()
{


    int o = rand() % 120;
    if(mouse_x <= 100 && mouse_y <= 100 && mouse_b & 1){
       //m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
       if(o<=30)
        {std::cout << "Ici" << std::endl;
        add_interfaced_vertex(o, 30.0, 100,100,"baleine.jpg");
        }
        if(o>=30&&o<=60)
        {
        add_interfaced_vertex(o, 30.0, 100,100,"dauphin.jpg");
        }
        if(o>=60&&o<=90)
        {
        add_interfaced_vertex(o, 30.0, 100,100,"plastique.jpg");
        }
        if(o>960&&o<=120)
        {
        add_interfaced_vertex(o, 30.0, 100,100,"humain.jpg");
        }
    }
}

void Graph::Aretecreer()
{
    int n, m;
    int i = rand() % 1010;

    if(mouse_x >= 900 && mouse_y <= 100 && mouse_b & 1)
    {

        std::cout << "Ici" << std::endl;
        std::cout << "le predateur:" << std::endl;
        std::cin >> n;
        std::cout << "La proie :" << std::endl;
        std::cin >> m;

        add_interfaced_edge(i, n, m, 25.0);
    }

}


/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx) != m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
}
/*
void Graph::WriteOnFile(std::string nom_fichier) ///ecrire et save
{
    std::ofstream file(nom_fichier.c_str(), std::ios::out);

    if(file){
        file << m_ordre << '\n';
        for(int i = 0, a = 0; i < 6; i++){
            for(int j = 0; j < 6; j++) file << a << ' ';
            file << '\n';
        }
    }
    else std::cout << "Couldn't open the file" << std::endl;
}

void  Graph::LireMatricePondere(std::string nom_fichier)
{
    std::vector<int> buf;

    std::ifstream fichier (nom_fichier.c_str(), std::ios::in);
    if(fichier){
        fichier >> m_ordre;
        for(int i = 0, a = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                fichier >> a;
                buf.push_back(a);
            }
            matrice.push_back(buf);
            buf.erase(buf.begin(), buf.end());
        }
        while(getline(fichier,ligne))  //on récupère une à une les lignes
        {
            if(i == 0){
                m_ordre= atoi(ligne.c_str());
                matrice=new int*[m_ordre]; //allocation de la premiere dimension
                for(int i=0; i<m_ordre; i++)
                {
                    matrice[i]=new int[m_ordre];//allocation de la deuxieme dimension
                }
            }

            else if(i>=1)
            {

                for(int j=0; j<m_ordre; j++)
                {
                    matrice[i-1][j]=ligne[j]-48;
                }
            }
            i++;
        }*/
        //fichier.close();
   // }
    //else
       // std::cout << "Erreur à l'ouverture fichier matrice ponderee!" << std::endl;
//

