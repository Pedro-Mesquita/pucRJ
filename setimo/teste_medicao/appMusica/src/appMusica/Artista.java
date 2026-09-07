package appMusica;

import java.util.ArrayList;
import java.util.Date;

public class Artista {
	private String nomeArtistico;
	private String nomeNascenca;
	private Date dataNascimento;
	private ArrayList<Album> albuns;
	
	public int getQtdeAlbuns() {
		return albuns.size();
	}
	
	public int getQtdeMusicas() {
		//TODO: Implementar função
		return 0;
	}

	public String getNomeArtistico() {
		return nomeArtistico;
	}

	public void setNomeArtistico(String nomeArtistico) {
		this.nomeArtistico = nomeArtistico;
	}

	public String getNomeNascenca() {
		return nomeNascenca;
	}

	public void setNomeNascenca(String nomeNascenca) {
		this.nomeNascenca = nomeNascenca;
	}

	public Date getDataNascimento() {
		return dataNascimento;
	}

	public void setDataNascimento(Date dataNascimento) {
		this.dataNascimento = dataNascimento;
	}

	public ArrayList<Album> getAlbums() {
		return albuns;
	}

	public void setAlbums(ArrayList<Album> albums) {
		this.albuns = albums;
	}
	
	
}