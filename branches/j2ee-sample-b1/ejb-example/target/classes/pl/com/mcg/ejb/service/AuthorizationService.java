package pl.com.mcg.ejb.service;

public interface AuthorizationService {
	String getAdminWelcome();

	String getUserWelcome();

	String getAnonymousWelcome();
}
