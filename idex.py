from bs4 import BeautifulSoup
from requests import get
from sqlite3 import connect
from scrapy import Selector
from wappalyzer import Wappalyzer

def obtener_titulo(url):
  """Extrae el título de una página web."""
  try:
    respuesta = get(url, headers={"User-Agent": "Mozilla/5.0"})
    respuesta.raise_for_status()
    sopa = BeautifulSoup(respuesta.content, "html.parser")
    return sopa.title.string
  except Exception as e:
    print(f"Se produjo un error al obtener el título: {e}")

def obtener_subdominios(url):
  """Extrae los subdominios de una página web."""
  subdominios = set()
  try:
    respuesta = get(url, headers={"User-Agent": "Mozilla/5.0"})
    respuesta.raise_for_status()
    selector = Selector(respuesta.content)
    for enlace in selector.css("a"):
      href = enlace.get("href")
      if href and "://" in href:
        subdominio = href.split("://")[1].split("/")[0]
        subdominios.add(subdominio)
  except Exception as e:
    print(f"Se produjo un error al obtener los subdominios: {e}")
  return subdominios

def obtener_elementos(url):
  """Extrae elementos de interés de una página web."""
  elementos = []
  try:
    respuesta = get(url, headers={"User-Agent": "Mozilla/5.0"})
    respuesta.raise_for_status()
    selector = Selector(respuesta.content)
    for elemento in selector.css("p"):
      texto = elemento.text
      if texto and len(texto) > 50:
        elementos.append(texto)
  except Exception as e:
    print(f"Se produjo un error al obtener los elementos: {e}")
  return elementos

def obtener_metadatos(url):
  """Extrae metadatos de una página web."""
  metadatos = {}
  try:
    respuesta = get(url, headers={"User-Agent": "Mozilla/5.0"})
    respuesta.raise_for_status()
    selector = Selector(respuesta.content)
    for meta in selector.css("meta"):
      nombre = meta.get("name")
      contenido = meta.get("content")
      if nombre and contenido:
        metadatos[nombre] = contenido
  except Exception as e:
    print(f"Se produjo un error al obtener los metadatos: {e}")
  return metadatos

def obtener_tecnologias(url):
  """Extrae las tecnologías utilizadas en una página web."""
  tecnologias = set()
  try:
    respuesta = get(url, headers={"User-Agent": "Mozilla/5.0"})
    respuesta.raise_for_status()
    wappalyzer = Wappalyzer()
    tecnologias.update(wappalyzer.analyze(respuesta.content))
  except Exception as e:
    print(f"Se produjo un error al obtener las tecnologías: {e}")
  return tecnologias

def guardar_informacion(url, titulo, subdominios, elementos, metadatos, tecnologias):
  """Guarda la información de una página web en una base de datos."""
  conexion = connect("webs_visitadas.db")
  cursor = conexion.cursor()
  try:
    cursor.execute("""
    INSERT INTO webs (url, titulo, subdominios, elementos, metadatos, tecnologias)
    VALUES (?, ?, ?, ?, ?, ?)
    """, (url, titulo, ",".join(subdominios), ",".join(elementos), ",".join(metadatos), ",".join(tecnologias)))
    conexion.commit()
  except Exception as e:
    print(f"Se produjo un error al guardar la información: {e}")
  finally:
    conexion.close()

def main():
  """Función principal del programa."""
  url = "https://www.ejemplo.com/"
  titulo = obtener_titulo(url)
  subdominios = obtener_subdominios(url)
  elementos = obtener_elementos(url)
  metadatos = obtener_metadatos(url)
  tecnologias = obtener_tecnologias(url)

  # Imprimir resultados
  print(
  print(f"**Título:** {titulo}")
  print(f"**Subdominios:** {', '.join(subdominios)}")
  print(f"**Elementos de interés:** {', '.join(elementos)}")
  print(f"**Metadatos:** {', '.join(['%s: %s' % (k, v) for k, v in metadatos.items()])}")
  print(f"**Tecnologías:** {', '.join(tecnologias)}")

  # Guardar en base de datos
  guardar_informacion(url,} titulo, subdominios, elementos, metadatos, tecnologias)

if __name__ == "__main__":
  main()