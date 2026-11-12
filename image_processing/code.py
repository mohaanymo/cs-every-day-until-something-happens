from PIL import Image
import matplotlib.pyplot as plt
import numpy as np


image = Image.open("./assets/contrast-sample.jpeg").convert("L")
img = np.array(image)

"""
    Negative
flips white to dark and dark to white
the formula is => s = 255 - r
"""

# img = 255 - img


"""
    Log transform
makes dark values more distinguishable while compressing large brightness values.

formula: s = c * log(1+r) where c = 255/log(1+R)
"""

# find the max pixel value
R = img.max()

c = 255 / np.log10(1 + R)
log_image = c * np.log10(1 + img)
log_image = log_image.astype(np.uint8)


"""
Power / Gamma Transform

Gamma transformation changes image brightness nonlinearly.

formula: s = (r^gamma)scaled 
we must first scale pixel value from 0-255 to 0-1
"""

gamma = 1.5
scaled = img / 255.0
gamma_img = scaled**gamma
gamma_img = gamma_img * 255
gamma_img = gamma_img.astype(np.uint8)


"""
Contrast Stretching

some images theirs pixels are consuntrated to a narrow range,
those images usally appear gray and flat because it doesn't use the full brightness range

the contrast stretching expands the whole range to fit 0-255

formula: s = (r - r_min)/(r_max-r_min) * 255
"""

r_min = img.min()
r_max = img.max()
shifted = img - r_min
org_range = r_max - r_min
stretched = shifted / org_range * 255

stretched = stretched.astype(np.uint8)


# Load all images
images = []
import glob

for file in glob.glob("assets/*.png"):
    print(file)
    if "vlcsnap" in str(file):
        image = np.array(Image.open(file).convert("L"))

        images.append(image)

# Convert the list into one NumPy array
images = np.array(images, dtype=np.float32)

# Average along the image dimension
average = images.mean(axis=0)

# Convert to 8-bit
average = average.astype(np.uint8)


plt.imshow(average, cmap="gray")
plt.savefig("my_plot.png")
plt.show()
