#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    std::string fichierprim;
    std::string fichierdijkstra;

    Graph mongraphe;
    Graph mongraphe2;

    srand(time(NULL));




    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    //Graph g;
    //g.make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )

    BITMAP *rougee= NULL;
    BITMAP *buffer = NULL;
    BITMAP* menu = NULL;
    buffer = create_bitmap(SCREEN_W,SCREEN_H);
    menu = load_bitmap("menu.bmp",NULL);
    rougee = load_bitmap("rouge.jpg",NULL);

    mongraphe2.make_example();
    int x=1;

    while ( !key[KEY_ESC] )
    {
while(x=1) /// Boucle d'interaction
    {
          masked_blit(menu, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rectfill(buffer, 0, 0, 40, 40, makecol(255,0,0)); /// Dessiner un rectangle
        rectfill(buffer, 300, 300, 100, 100, makecol(255,0,0));

                if(mouse_x <40 && mouse_y < 40 && mouse_b & 1)
                    exit(0);

          if(mouse_x <= 400 &&  mouse_y <= 400 && mouse_b & 1)
        {
        while ( !key[KEY_ESC] )
    {
    /*
    int t=0;
    int h=0;
    h++;

    if(h%7==0)
    {
      t++;
    }
    */
        masked_blit(rougee, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        masked_blit(rougee, screen, 0, 0, 900, 0, SCREEN_W, SCREEN_H);

        mongraphe2.update();
        mongraphe2.create();
        mongraphe2.Aretecreer();

        grman::mettre_a_jour();
        }
      }
    }
}

    //mongraphe2.WriteOnFile("unfichier.txt");
    grman::fermer_allegro();


    return 0;
}
END_OF_MAIN();
