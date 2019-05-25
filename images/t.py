# -*- coding: utf-8 -*-
import pyglet

window = pyglet.window.Window()
image = pyglet.resource.image('small.jpg')

@window.event
def on_draw():
    window.clear()
    image.blit(0, 0)
    pyglet.graphics.draw(2, pyglet.gl.GL_POINTS,
    ('v2i', (10, 15, 30, 35))
)

pyglet.app.run()
