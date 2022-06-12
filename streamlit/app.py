import streamlit as st
import pandas as pd
import imageio
import numpy as np
from PIL import Image
from io import BytesIO
import matplotlib.pyplot as plt
import pickle
import os
import cv2
from time import sleep
from streamlit_drawable_canvas import st_canvas
st.set_option('deprecation.showPyplotGlobalUse', False)

# Title and Subtitle
st.title('Imagine')
st.subheader('Visualizador de Filtros')


def show_image():
    img = imageio.imread('../src/out/streamlit_prueba.ppm')
    plt.imshow(img)
    st.pyplot()


def run_filters(c_shades, c_crop_v, c_crop_h, c_zoom, c_threads, merge_totoro, merge_cebra, merge_chihiro, merge_mononoke, blackAndWhite, shades, zoom, crop, edgeDetection):
    command = f'../src/main '
    if blackAndWhite: 
        command += f'BYN {c_threads} 0 ../src/imgs/ashitaka.ppm ../src/out/streamlit_prueba.ppm'
    elif merge:

        img_merge = 'totoro.ppm'
        if merge_totoro:
            img_merge = 'totoro.ppm'
        elif merge_cebra:
            img_merge = 'cebra.ppm'
        elif merge_chihiro:
            img_merge = 'chihiro.ppm'
        elif merge_mononoke:
            img_merge = 'mononoke.ppm'

        command += f'merge 1 0.5 ../src/imgs/ashitaka.ppm ../src/out/streamlit_prueba.ppm 0 ../src/imgs/{img_merge}'
    elif shades:
        command += f'shades 1 {c_shades} ../src/imgs/ashitaka.ppm ../src/out/streamlit_prueba.ppm'
    elif zoom:
        command += f'zoom 1 {c_zoom} ../src/imgs/ashitaka.ppm ../src/out/streamlit_prueba.ppm'
    elif crop:
        command += f'crop 1 {c_crop_v} ../src/imgs/ashitaka.ppm ../src/out/streamlit_prueba.ppm {c_crop_h}'
    elif edgeDetection:
        command += f'ed {c_threads} 0 ../src/imgs/ashitaka.ppm ../src/out/streamlit_prueba.ppm {c_crop_h}'

    os.system(command)


# Filtros de Sidebar
st.sidebar.header('Filtros')
blackAndWhite = st.sidebar.checkbox('Blanco y Negro', value = False)
merge = st.sidebar.checkbox('Merge', value = False)
shades = st.sidebar.checkbox('Shades', value = False)
zoom = st.sidebar.checkbox('Zoom', value = False)
crop = st.sidebar.checkbox('Crop', value = False)
edgeDetection = st.sidebar.checkbox('Edge Detection', value = False)


c_shades = None
c_crop_v = None
c_crop_h = None
c_zoom = None
c_threads = None

merge_totoro = False
merge_cebra = False
merge_chihiro = False
merge_mononoke = False

# Opciones de filtros
if blackAndWhite:
    c_threads = st.sidebar.slider('Cantidad de threads', 1, 32, 1, 1, '%d')
elif merge:
    st.sidebar.write("--")
    merge_totoro = st.sidebar.checkbox('Totoro', value = False)
    merge_cebra = st.sidebar.checkbox('Cebra', value = False)
    merge_chihiro = st.sidebar.checkbox('Chihiro', value = False)
    merge_mononoke = st.sidebar.checkbox('Mononoke', value = False)
elif shades:
    c_shades = st.sidebar.slider('Cantidad de sombras', 1, 255, 0, 1, '%d')
elif zoom:
    c_zoom = st.sidebar.slider('Cantidad de zoom', 1, 4, 0, 1, '%d')
elif crop:
    c_crop_v = st.sidebar.slider('Recorte Vertical', 100, 500, 0, 1, '%d')
    c_crop_h = st.sidebar.slider('Recorte Horizontal', 100, 500, 0, 1, '%d')
elif edgeDetection:
    c_threads = st.sidebar.slider('Cantidad de threads', 1, 32, 1, 1, '%d')

# Event Listener de aplicar
if st.sidebar.button('Aplicar'):
    run_filters(c_shades, c_crop_v, c_crop_h, c_zoom, c_threads, merge_totoro, merge_cebra, merge_chihiro, merge_mononoke, blackAndWhite, shades, zoom, crop, edgeDetection)
    sleep(3)
    show_image()