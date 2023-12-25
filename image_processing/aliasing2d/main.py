import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter
from PIL import Image

if __name__ == '__main__':
    resolution = 256
    M = 64
    lines_per_frame = 16
    num_blades = 3

    x = np.linspace(0, 2 * np.pi, 1000)
    i = 0

    # Create plot with 256x256 resolution
    first_fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}, figsize=(2.6, 2.6))
    l, = plt.plot([], [], '-')
    ax.tick_params(axis='y', labelleft=False)
    plt.grid(color='grey', linestyle='--', linewidth=0.5)
    shutter_effect = Image.new('RGB', (resolution, resolution))

    def update(m):
        r = np.sin(num_blades * x + (m * np.pi) / 10)
        l.set_data(x, r)

        global i
        plt.savefig('frame.png')
        shutter_effect.paste(
            Image.open('frame.png').crop((0, i * lines_per_frame, resolution, i * lines_per_frame + lines_per_frame)),
            (0, i * lines_per_frame))
        i += 1


    FuncAnimation(first_fig, update, np.arange(-M / 2, M / 2, 1)).save("spinning_blades.gif",
                                                                         writer=PillowWriter(fps=16))
    shutter_effect.save("shutter_effect.png")

