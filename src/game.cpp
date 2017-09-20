/*
 * game.cpp
 * humrcraft Source Code
 * Available on Github
 *
 * Copyright (C) 2017 Karol "digitcrusher" Łacina
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "game.hpp"
#include "renderers.hpp"

Thing::Thing(void* data, size_t datasize, void (*constructorf)(), void (*destructorf)(), Shape* shape, float health, float damage, GLuint textureid) : Object(shape) {
    this->family.pushBack("Thing");
    this->maxhealth = health;
    this->health = this->maxhealth;
    this->damage = damage;
    this->textureid = textureid;
    this->head = NULL;
    this->rhand = NULL;
    this->lhand = NULL;
    this->chest = NULL;
    this->back = NULL;
    this->legs = NULL;
    this->feet = NULL;
    this->data = data;
    this->datasize = datasize;
    this->constructorf = constructorf;
    this->destructorf = destructorf;
    this->updatef = NULL;
    this->renderf = NULL;
    this->speakf = NULL;
    this->usef = NULL;
    this->attackf = NULL;
    this->actionf = NULL;
    this->collisionCallbackf = NULL;
    if(this->constructorf) {
        this->constructorf();
    }
}
Thing::~Thing() {
    if(this->destructorf) {
        this->destructorf();
    }
}
void Thing::update(double delta) {
    Object::update(delta);
    if(this->updatef) {
        this->updatef(delta);
    }
}
void Thing::render(Renderer* renderer) {
    Object::render(renderer);
    if(!this->checkFamily(renderer, "SDLGLRenderer", 2)) return;
    if(this->textureid) {
        glBindTexture(GL_TEXTURE_2D, this->textureid);
        glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBegin(GL_TRIANGLES);
                    V2f pos = ((SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x-(float)1/2, this->getPos().y-(float)1/2}, {0, 0});
                    glTexCoord2f(0, 1);
                    glVertex3f(pos.x, pos.y, 1);
                    pos = ((SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x+(float)1/2, this->getPos().y-(float)1/2}, {0, 0});
                    glTexCoord2f(1, 1);
                    glVertex3f(pos.x, pos.y, 1);
                    pos = ((SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x-(float)1/2, this->getPos().y+(float)1/2}, {0, 0});
                    glTexCoord2f(0, 0);
                    glVertex3f(pos.x, pos.y, 0);

                    pos = ((SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x+(float)1/2, this->getPos().y+(float)1/2}, {0, 0});
                    glTexCoord2f(1, 0);
                    glVertex3f(pos.x, pos.y, 1);
                    pos = ((SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x+(float)1/2, this->getPos().y-(float)1/2}, {0, 0});
                    glTexCoord2f(1, 1);
                    glVertex3f(pos.x, pos.y, 1);
                    pos = ((SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x-(float)1/2, this->getPos().y+(float)1/2}, {0, 0});
                    glTexCoord2f(0, 0);
                    glVertex3f(pos.x, pos.y, 0);
                glEnd();
            glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if(this->renderf) {
        this->renderf(renderer);
    }
}
void Thing::speak(Speaker* speaker) {
    Object::speak(speaker);
    if(this->speakf) {
        this->speakf(speaker);
    }
}
void Thing::use() {
    if(this->usef) {
        this->usef();
    }
}
void Thing::attack() {
    if(this->attackf) {
        this->attackf();
    }
}
void Thing::action(const char* action) {
    if(!strcmp(action, "use")) {
        this->use();
    }else if(!strcmp(action, "attack")) {
        this->attack();
    }
    if(this->actionf) {
        this->actionf(action);
    }
}
void Thing::collisionCallback(manifold* manifold) {
    Object::collisionCallback(manifold);
    if(this->collisionCallbackf) {
        this->collisionCallbackf(manifold);
    }
}

Gun::Gun(void* data, size_t datasize, void (*constructorf)(), void (*destructorf)(), Shape* shape, float health, GLuint textureid) :
    Thing(data, datasize, constructorf, destructorf, shape, health, 0, textureid) {
    this->family.pushBack("Gun");
}
Gun::~Gun() {
}
/*void Gun::use() {
    if(this->world && this->sample) {
        this->world->add(new Projectile(this->sample->data, this->sample->datasize, this->sample->constructorf, this->sample->destructorf, this->sample->shape));
    }
}*/

Projectile::Projectile(void* data, size_t datasize, void (*constructorf)(), void (*destructorf)(), Shape* shape, float health, float damage, float speed, GLuint textureid) :
Thing(data, datasize, constructorf, destructorf, shape, health, damage, textureid) {
    this->family.pushBack("Projectile");
    this->speed = speed;
}
Projectile::~Projectile() {
}
void Projectile::update(double delta) {
    Thing::update(delta);
}
void Projectile::render(Renderer* renderer) {
    Thing::render(renderer);
    glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 0, 1);
        glBegin(GL_LINES);
            V2f pos = ((SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x, this->getPos().y}, {0, 0});
            glTexCoord2f(0, 1);
            glVertex3f(pos.x, pos.y, 1);
            pos = ((SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x, this->getPos().y}, {0, 0});
            glTexCoord2f(1, 1);
            glVertex3f(pos.x, pos.y, 1);
        glEnd();
    glDisable(GL_BLEND);
}
void Projectile::collisionCallback(manifold* manifold) {
    Thing::collisionCallback(manifold);
    Object* hit = manifold->a==this?manifold->b:manifold->a;
    if(hit->checkFamily(hit, "Thing", 1)) {
        ((Thing*)hit)->health -= this->damage;
    }
    if(this->world) {
        this->world->remove(this->id);
    }
    if(!this->shared) {
        delete this;
    }
}
