import React, { useState, useEffect } from 'react';
import {ActivityIndicator, Alert} from 'react-native';
import { useSelector } from 'react-redux';
import Icon from 'react-native-vector-icons/MaterialIcons';
import {withNavigationFocus} from 'react-navigation';

import api from '~/services/api';

import Background from '~/Components/Background';

import {Container, Title, Button, ButtonsContainer, Text} from './styles';

function Dashboard() {  
  const ip = useSelector(state => state.user.controllerIP);

  async function handlePress(option) {
    try {   
      await api.post(ip, option);          
    } catch (error) {
      Alert.alert('Erro', error.message);
    }
  }

  return (
    <Background>
      <Container>
        <Title>Corredor</Title>

        <ButtonsContainer>          
          <Button onPress={() => handlePress('ligar')}>
            <Text>Acender lâmpadas</Text>
          </Button>
          <Button onPress={() => handlePress('desligar')}>
            <Text>Apagar lâmpadas</Text>
          </Button>          
        </ButtonsContainer>
      </Container>
    </Background>
  );
}

Dashboard.navigationOptions = {
  tabBarLabel: 'Corredor',
  tabBarIcon: ({tintColor}) => (
    <Icon name="home" size={20} color={tintColor} />
  ),
};

export default withNavigationFocus(Dashboard);
