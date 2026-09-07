package appMusica;

import java.util.ArrayList;

public class Album {
	private String titulo;
	private int anoLancamento;
	private ArrayList<Musica> playlist;
	
	
	int getQtdeMusicas() {
		return this.playlist.size();
	}

	//Getters and setters
	public String getTitulo() {
		return titulo;
	}


	public void setTitulo(String titulo) {
		this.titulo = titulo;
	}


	public int getAnoLancamento() {
		return anoLancamento;
	}


	public void setAnoLancamento(int anoLancamento) {
		this.anoLancamento = anoLancamento;
	}


	public ArrayList<Musica> getPlaylist() {
		return playlist;
	}


	public void setPlaylist(ArrayList<Musica> playlist) {
		this.playlist = playlist;
	}
}