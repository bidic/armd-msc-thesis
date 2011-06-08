package pl.com.mcg.ejb.remote;

import javax.ejb.Remote;

import pl.com.mcg.ejb.service.AuthorizationService;

@Remote
public interface AuthorizationServiceRemote extends AuthorizationService {
}