package pl.com.mcg.ejb.service;

import java.util.Collection;

public interface SessionBeanService {

	String getTime();

	void addItem(String text);

	Collection<String> getItems();

}
