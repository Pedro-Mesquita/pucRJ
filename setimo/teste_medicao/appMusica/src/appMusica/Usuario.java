package appMusica;

import java.util.ArrayList;

public class Usuario {
	private String login;
	private String senha;
	private ArrayList<Musica> musicasFavoritdas = new ArrayList<Musica>();
	private ArrayList<Playlist> playlists = new ArrayList<Playlist>();

	Usuario(String login, String senha) {
		this.login = login;
		this.senha = senha;
	}
	
	public boolean favoritar(Musica musica) {
//		TODO: Implementar função
		return true;
	}
	
	public boolean escutar(Musica musica) {
//		TODO: Implementar função
		return true;
	}
	
	public boolean criarPlaylist(String nome) {
//		TODO: Implementar função
		return true;
	}
	
	//Getters and setters:
	
	public String getLogin() {
		return login;
	}

	public void setLogin(String login) {
		this.login = login;
	}

	public String getSenha() {
		return senha;
	}

	public void setSenha(String senha) {
		this.senha = senha;
	}

	public ArrayList<Musica> getMusicasFavoritdas() {
		return musicasFavoritdas;
	}

	public void setMusicasFavoritdas(ArrayList<Musica> musicasFavoritdas) {
		this.musicasFavoritdas = musicasFavoritdas;
	}

	public ArrayList<Playlist> getPlaylists() {
		return playlists;
	}

	public void setPlaylists(ArrayList<Playlist> playlists) {
		this.playlists = playlists;
	}
}