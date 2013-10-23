using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Text;

public class GigapanImageScraping : MonoBehaviour {
	public GameObject imgPlane;
	public bool isGigaPan = true;
	public GameObject zoomGUI;
	public GameObject[] TileGen;
	static public int zoomLevel = 7, currX = 20, currY = 20;
	private string baseURL = ".";
	private string fileExt = "jpg";
	private Color colorEnd = Color.white;
	private bool keyEnable = true;
	private Vector3 tilePos = new Vector3(0,0,0);
	private bool[] tilesDone = new bool[5] {false, false, false, false, false};
	static private int tileWidth = 10;
	static private double panWidth = 246156, panHeight = 42600, levels = 11;
	static private int scrWidth = 60, scrHeight = 10;
	private GameObject[][,] Tiles = new GameObject[5][,];
	//private Texture2D[,,] cachedTiles = new Texture2D[5,scrHeight,scrWidth];
	private int nbrCountX = 8, nbrCountY = 12;
	public bool isFetching = false;
	private LayerMask lm = new LayerMask();
	public bool ismousePressed = false;
	
	void Awake () {
		Screen.fullScreen = true;
		Security.PrefetchSocketPolicy("128.2.207.73", 843);
		Security.PrefetchSocketPolicy("128.2.220.80", 843);
		Security.PrefetchSocketPolicy("128.2.220.81", 843);
	}
	
	// Use this for initialization
	void Start () {
		Screen.fullScreen = true;
		lm.value = 1<<8;
		
		//Spawing new layer of tiles
		int xph = (int)(((zoomLevel - 6) == 0) ? 1 :(1 << (zoomLevel - 6))) * scrHeight;
		int xpw = (int)(((zoomLevel - 6) == 0) ? 1 :(1 << (zoomLevel - 6))) * scrWidth;
		Tiles[zoomLevel - 6] = new GameObject[xph,xpw];
		tilePos.x = TileGen[zoomLevel - 6].transform.position.x;
		tilePos.y = TileGen[zoomLevel - 6].transform.position.y;
		tilePos.z = TileGen[zoomLevel - 6].transform.position.z;
		for (int i = 0; i < xph; i++) {
			for (int j = 0; j < xpw; j++) {
				tilePos.x += tileWidth;
				Tiles[zoomLevel - 6][i,j] = (GameObject) Instantiate(TileGen[zoomLevel - 6],
                                          tilePos,
                                          TileGen[zoomLevel - 6].transform.rotation
                                          );
				
				Tiles[zoomLevel - 6][i,j].GetComponent<Tile>().x = j;
				Tiles[zoomLevel - 6][i,j].GetComponent<Tile>().y = i;
				Tiles[zoomLevel - 6][i,j].GetComponent<Tile>().zoomLvl = zoomLevel;
			}
			tilePos.y -= tileWidth;
			tilePos.x = 0;
		}
		tilesDone[zoomLevel - 6] = true;
	}
	
	#region Controller
	
	void initZoomLevel(bool isZoom) {
		//Disabling previous layer of tiles
		
		if (isZoom) {
			int xpdh = (int)(((zoomLevel - 7) == 0) ? 1 :(1 << (zoomLevel - 7))) * scrHeight;
			int xpdw = (int)(((zoomLevel - 7) == 0) ? 1 :(1 << (zoomLevel - 7))) * scrWidth;
			for (int i = 0; i < xpdh; i++) {
				for (int j = 0; j < xpdw; j++) {
					Tiles[zoomLevel - 6-1][i,j].active = false;
				}
			}
		}
		else {
			int xpdh = (int)(((zoomLevel - 5) == 0) ? 1 :(1 << (zoomLevel - 5))) * scrHeight;
			int xpdw = (int)(((zoomLevel - 5) == 0) ? 1 :(1 << (zoomLevel - 5))) * scrWidth;
			for (int i = 0; i < xpdh; i++) {
				for (int j = 0; j < xpdw; j++) {
					Tiles[zoomLevel - 6+1][i,j].active = false;
				}
			}
		}
		int xph = (int)(((zoomLevel - 6) == 0) ? 1 :(1 << (zoomLevel - 6))) * scrHeight;
		int xpw = (int)(((zoomLevel - 6) == 0) ? 1 :(1 << (zoomLevel - 6))) * scrWidth;
		
		//Enabling new layer of tiles.
		if (isZoom && tilesDone[zoomLevel - 6]) {
			for (int i = 0; i < xph; i++) {
				for (int j = 0; j < xpw; j++) {
					Tiles[zoomLevel - 6][i,j].active = true;
				}
			}
		}
		else if (!isZoom && tilesDone[zoomLevel - 6]) {
			for (int i = 0; i < xph; i++) {
				for (int j = 0; j < xpw; j++) {
					Tiles[zoomLevel - 6][i,j].active = true;
				}
			}
		}
		else {
			//Spawing new layer of tiles
			Tiles[zoomLevel - 6] = new GameObject[xph,xpw];
			tilePos.x = TileGen[zoomLevel - 6].transform.position.x;
			tilePos.y = TileGen[zoomLevel - 6].transform.position.y;
			tilePos.z = TileGen[zoomLevel - 6].transform.position.z;
			for (int i = 0; i < xph; i++) {
				for (int j = 0; j < xpw; j++) {				
					tilePos.x += tileWidth;
					Tiles[zoomLevel - 6][i,j] = (GameObject) Instantiate(TileGen[zoomLevel - 6],
	                                          tilePos,
	                                          TileGen[zoomLevel - 6].transform.rotation
	                                          );
					
					Tiles[zoomLevel - 6][i,j].GetComponent<Tile>().x = j;
					Tiles[zoomLevel - 6][i,j].GetComponent<Tile>().y = i;
					Tiles[zoomLevel - 6][i,j].GetComponent<Tile>().zoomLvl = zoomLevel;
				}
				tilePos.y -= tileWidth;
				tilePos.x = 0;
			}
			tilesDone[zoomLevel - 6] = true;
		}
	}
	
	void CamLocScale(bool isZoom) {
		Vector3 oldPos = Camera.mainCamera.transform.position;
		float percX = 0.0f, percY = 0.0f;
		Debug.Log("Prev Width : "+ (scrWidth * Mathf.Pow(2, (zoomLevel - 6 - 1))));
		Debug.Log("Prev Height : "+ (scrHeight * Mathf.Pow(2, (zoomLevel - 6 - 1))));
		//Get scale percentage of old X-Y position
		if (isZoom) {
			percX = (float)(Camera.mainCamera.transform.position.x/10)/(scrWidth * Mathf.Pow(2, (zoomLevel - 6 - 1)));
			percY = (float)(Mathf.Abs(Camera.mainCamera.transform.position.y)/10)/(scrHeight * Mathf.Pow(2, (zoomLevel - 6 - 1)));
		}
		else {
			percX = (float)(Camera.mainCamera.transform.position.x/10)/(scrWidth * Mathf.Pow(2, (zoomLevel - 6 + 1)));
			percY = (float)(Mathf.Abs(Camera.mainCamera.transform.position.y)/10)/(scrHeight * Mathf.Pow(2, (zoomLevel - 6 + 1)));
		}
		Debug.Log(percX + " " + percY);
		//Multiply that scale to new X-Y position
		Debug.Log("old Pow : "+ (int)(((zoomLevel - 6) == 0) ? 1 :(1 << (zoomLevel - 6))));
		Debug.Log("new Pow : "+ (int)(((zoomLevel - 6) == 0) ? 1 :(1 << (zoomLevel - 6))));
		
		oldPos.x = scrWidth * (int)(((zoomLevel - 6) == 0) ? 1 :(1 << (zoomLevel - 6))) * percX * 9.5f;
		oldPos.y = -scrHeight * (int)(((zoomLevel - 6) == 0) ? 1 :(1 << (zoomLevel - 6))) * percY * 10;
		Debug.Log("New Width : "+ (scrWidth * Mathf.Pow(2, (zoomLevel - 5))));
		Debug.Log("New Width : "+ (scrHeight * Mathf.Pow(2, (zoomLevel - 5))));
		Debug.Log(oldPos.x + " " + oldPos.y);
		if (isZoom) {
			oldPos.z += 40;
		}
		else {
			oldPos.z -= 40;
		}
		Camera.mainCamera.transform.parent.transform.position = oldPos;
	}
	
	
	void OnGUI () {
		zoomGUI.guiText.text = (zoomLevel - 6).ToString();
	}
	
	#endregion Controller
	
	#region Scrape
	
	public void FetchOneTile (int x, int y, int zoomLvl) {
		if (!isFetching) {
			StartCoroutine(getGigaTile(x, y, zoomLevel));
		}
		isFetching = false;
	}
	
	public void DelTex (int x, int y, int zoomLvl) {
		if (Tiles[zoomLevel - 6][y, x].GetComponent<Tile>().hasTex) {
			Object.Destroy(Tiles[zoomLevel - 6][y, x].renderer.material.mainTexture);
			Object.Destroy(Tiles[zoomLevel - 6][y, x].renderer.material);
			Tiles[zoomLevel - 6][y, x].GetComponent<Tile>().hasTex = false;
			Destroy(Tiles[zoomLevel - 6][y, x].GetComponent<Tile>().tileTex);
		}
	}
	
	IEnumerator FetchTile () {
		if (!isFetching) {
			isFetching = true;
			int x =(int) Camera.mainCamera.transform.position.x / 10;
			int y =(int) Mathf.Abs(Camera.mainCamera.transform.position.y) / 10;
			for (int i = -nbrCountX ; i < nbrCountX; i++) {
				for (int j = -nbrCountY/2; j < nbrCountY/2; j++) {
					StartCoroutine(getGigaTile(x + j, y + i, zoomLevel));
				}
			}
			keyEnable = true;
		}
		isFetching = false;
		yield return null;
	}
	
public IEnumerator getGigaTile(int x, int y, int zoom) {
	if (!Tiles[zoomLevel - 6][y, x].GetComponent<Tile>()) {
		Tiles[zoomLevel - 6][y, x].AddComponent<Tile>();
	}
	if (!Tiles[zoomLevel - 6][y, x].GetComponent<Tile>().hasTex) {
		string code = MakeGigaURL(x, y, zoom);
		//Debug.Log("x : " + x + " y: " + y + " idx : " + zoom);
		//Debug.Log(baseURL+code+"."+fileExt);
		WWW texObj = null;
	    texObj = new WWW(baseURL+code+"."+fileExt);
		//Index : (x - currX) * row + (y - currY)
	    yield return texObj;
		if (!Tiles[zoomLevel - 6][y, x].renderer.material.mainTexture) {
			Tiles[zoomLevel - 6][y, x].GetComponent<Tile>().hasTex = true;
			Tiles[zoomLevel - 6][y, x].GetComponent<Tile>().tileTex = texObj.texture;
			Tiles[zoomLevel - 6][y, x].renderer.material.mainTexture = texObj.texture;
			
			Tiles[zoomLevel - 6][y, x].renderer.material.shader = Shader.Find("Self-Illumin/Diffuse");
			StartCoroutine(FadeIn(Tiles[zoomLevel - 6][y, x]));
			texObj.Dispose();
			texObj = null;
		}
		else {
			Tiles[zoomLevel - 6][y, x].renderer.material.mainTexture = Tiles[zoomLevel - 6][y, x].GetComponent<Tile>().tileTex;
		}
	yield return null;
}
}
	
	#endregion Scrape
	
	#region Helpers
	
	public IEnumerator FadeIn(GameObject tile) {
		float timetravelled = 0.0f;
		while (timetravelled < 5.0f){
			timetravelled += Time.deltaTime;
			tile.renderer.material.SetColor("_Color", Color.Lerp(tile.renderer.material.GetColor("_Color"), colorEnd, Time.deltaTime));
			yield return null;
		}
	}
	
	public string MakeGigaURL (int x, int y, int zoom) {
		string quadKey = TileXYToQuadKey(x, y, zoom);
		
		StringBuilder url = new StringBuilder();
		url.Append(quadKey[0]);
		url.Append(quadKey[1]);
		url.Append('/');
		
		if ((zoom == 6) || (zoom == 7) || (zoom == 8) || (zoom == 9)) {
			url.Append(quadKey[2]);
			url.Append(quadKey[3]);
			url.Append(quadKey[4]);
			url.Append('/');
		}
		if (zoom == 9){
			url.Append(quadKey[5]);
			url.Append(quadKey[6]);
			url.Append(quadKey[7]);
			url.Append('/');
		}
		url.Append("r");
		url.Append(quadKey);
		return url.ToString();
	}
	
    public static void QuadKeyToTileXY(string quadKey, out int tileX, out int tileY, out int levelOfDetail)
    {
        tileX = tileY = 0;
        levelOfDetail = quadKey.Length;
        for (int i = levelOfDetail; i > 0; i--)
        {
            int mask = 1 << (i - 1);
            switch (quadKey[levelOfDetail - i])
            {
                case '0':
                    break;

                case '1':
                    tileX |= mask;
                    break;

                case '2':
                    tileY |= mask;
                    break;

                case '3':
                    tileX |= mask;
                    tileY |= mask;
                    break;

                default:
                    tileX = 0;
				    tileY = 0;
				    break;
            }
        }
    }
	
	public string TileXYToQuadKey(int tileX, int tileY, int levelOfDetail)
    {
		StringBuilder quadKey = new StringBuilder();
        for (int i = levelOfDetail; i > 0; i--)
        {
            char digit = '0';
            int mask = 1 << (i - 1);
            if ((tileX & mask) != 0)
            {
                digit++;
            }
            if ((tileY & mask) != 0)
            {
                digit++;
                digit++;
            }
            quadKey.Append(digit);
        }
        return quadKey.ToString();
    }
	
    public static void PixelXYToTileXY(int pixelX, int pixelY, out int tileX, out int tileY)
    {
        tileX = pixelX / 256;
        tileY = pixelY / 256;
    }
	
    public static void TileXYToPixelXY(int tileX, int tileY, out int pixelX, out int pixelY)
    {
        pixelX = tileX * 256;
        pixelY = tileY * 256;
    }
	
	#endregion Helpers
}