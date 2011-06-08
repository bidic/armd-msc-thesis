package pl.com.mcg.ejb.impl;

import javax.annotation.security.DeclareRoles;
import javax.annotation.security.PermitAll;
import javax.annotation.security.RolesAllowed;
import javax.ejb.Remote;
import javax.ejb.Stateless;

import pl.com.mcg.ejb.remote.AuthorizationServiceRemote;
import pl.com.mcg.ejb.service.AuthorizationService;

@Stateless
@DeclareRoles( { "USER", "ADMIN" })
@Remote(AuthorizationServiceRemote.class)
public class RemoteAuthorizationServiceBeanImpl implements AuthorizationService {
	@RolesAllowed("ADMIN")
	public String getAdminWelcome() {
		return "AuthorizationService: AccessLevel: ADMIN";
	}

	@PermitAll
	public String getAnonymousWelcome() {
		return "AuthorizationService: AccessLevel: PermitAll";
	}

	@RolesAllowed( { "ADMIN", "USER" })
	public String getUserWelcome() {
		return "AuthorizationService: AccessLevel: USER";
	}
}