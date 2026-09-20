package appMusica;

import java.util.ArrayList;

public class Playlist {
	private String nome;
	private ArrayList<Musica> musicas = new ArrayList<Musica>();
	
	boolean adicionarMusica(Musica musica) {
		this.musicas.add(musica);
		return true;
	}

	//Getters and setters
	public ArrayList<Musica> getMusicas() {
		return musicas;
	}

	public void setMusicas(ArrayList<Musica> musicas) {
		this.musicas = musicas;
	}

	public String getNome() {
		return nome;
	}

	public void setNome(String nome) {
		this.nome = nome;
	}
}