/*
 * game.cpp
 * humrcraft Source Code
 * Available on Github
 *
 * Copyright (c) 2017 Karol "digitcrusher" Łacina
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "game.hpp"
#include "renderers.hpp"

Thing::Thing(void* data, size_t datasize, void (*initf)(), void (*uninitf)(), Shape* shape, float health, float damage, GLuint textureid) : Object(shape) {
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
    this->initf = initf;
    this->uninitf = uninitf;
    this->updatef = NULL;
    this->renderf = NULL;
    this->speakf = NULL;
    this->usef = NULL;
    this->attackf = NULL;
    this->actionf = NULL;
    this->collisionCallbackf = NULL;
    if(this->initf) {
        this->initf();
    }
}
Thing::~Thing() {
    if(this->uninitf) {
        this->uninitf();
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

Gun::Gun(void* data, size_t datasize, void (*initf)(), void (*uninitf)(), Shape* shape, float health, GLuint textureid) :
    Thing(data, datasize, initf, uninitf, shape, health, 0, textureid) {
    this->family.pushBack("Gun");
}
Gun::~Gun() {
}
/*void Gun::use() {
    if(this->world && this->sample) {
        this->world->add(new Projectile(this->sample->data, this->sample->datasize, this->sample->initf, this->sample->uninitf, this->sample->shape));
    }
}*/

Projectile::Projectile(void* data, size_t datasize, void (*initf)(), void (*uninitf)(), Shape* shape, float health, float damage, float speed, GLuint textureid) :
Thing(data, datasize, initf, uninitf, shape, health, damage, textureid) {
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
